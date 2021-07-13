#include "atom/atom_basic.h"

#include <memory>

#include "box/box_group.h"
#include "graphic/graphic.h"
#include "env/env.h"

using namespace std;
using namespace tex;

/***************************************************************************************************
 *                                     basic atom implementation                                   *
 ***************************************************************************************************/

sptr<Box> MathFontAtom::createBox(Env& env) {
  env.selectMathFont(_fontName, _mathStyle);
  return StrutBox::empty();
}

sptr<Box> ScaleAtom::createBox(Env& env) {
  return sptrOf<ScaleBox>(_base->createBox(env), _sx, _sy);
}

sptr<Box> MathAtom::createBox(Env& env) {
  const auto fontStyle = env.fontStyle();
  env.removeFontStyle(FontStyle::rm);
  const auto style = env.style();
  // if parent style greater than "this style", that means the parent uses smaller font size,
  // then uses parent style instead
  if (_style > style) {
    env.setStyle(_style);
  }
  auto box = _base->createBox(env);
  env.addFontStyle(fontStyle);
  env.setStyle(style);
  return box;
}

sptr<Box> HlineAtom::createBox(Env& env) {
  const auto drt = env.ruleThickness();
  auto b = new RuleBox(drt, _width, _shift, _color, false);
  auto vb = new VBox();
  vb->add(sptr<Box>(b));
  vb->_type = AtomType::hline;
  return sptr<Box>(vb);
}

CumulativeScriptsAtom::CumulativeScriptsAtom(
  const sptr<Atom>& base, const sptr<Atom>& sub, const sptr<Atom>& sup
) {
  if (auto ca = dynamic_cast<CumulativeScriptsAtom*>(base.get()); ca != nullptr) {
    _base = ca->_base;
    ca->_sup->add(sup);
    ca->_sub->add(sub);
    _sup = ca->_sup;
    _sub = ca->_sub;
  } else if (auto sa = dynamic_cast<ScriptsAtom*>(base.get()); sa != nullptr) {
    _base = sa->_base;
    _sup = sptrOf<RowAtom>(sa->_sup);
    _sub = sptrOf<RowAtom>(sa->_sub);
    _sup->add(sup);
    _sub->add(sub);
  } else {
    _base = base;
    _sup = sptrOf<RowAtom>(sup);
    _sub = sptrOf<RowAtom>(sub);
  }
}

void CumulativeScriptsAtom::addSuperscript(const sptr<Atom>& sup) {
  _sup->add(sup);
}

void CumulativeScriptsAtom::addSubscript(const sptr<Atom>& sub) {
  _sub->add(sub);
}

sptr<Atom> CumulativeScriptsAtom::getScriptsAtom() const {
  return sptrOf<ScriptsAtom>(_base, _sub, _sup);
}

sptr<Box> CumulativeScriptsAtom::createBox(Env& env) {
  return ScriptsAtom(_base, _sub, _sup).createBox(env);
}

/*************************************** color atom implementation ********************************/

const color ColorAtom::_default = black;

ColorAtom::ColorAtom(const sptr<Atom>& atom, color bg, color c)
  : _background(bg), _color(c) {
  _elements = sptrOf<RowAtom>(atom);
}

void ColorAtom::defineColor(const string& name, color c) {
  _colors[name] = c;
}

sptr<Box> ColorAtom::createBox(Env& env) {
  const auto box = _elements->createBox(env);
  return sptrOf<ColorBox>(box, _color, _background);
}

sptr<Box> RomanAtom::createBox(Env& env) {
  if (_base == nullptr) return StrutBox::empty();
  // TODO nested style?
  return env.withFontStyle(
    FontStyle::rm,
    [&](auto& v) { return _base->createBox(v); }
  );
}

PhantomAtom::PhantomAtom(const sptr<Atom>& el) {
  if (el == nullptr) _elements = sptrOf<RowAtom>();
  else _elements = sptrOf<RowAtom>(el);
  _w = _h = _d = true;
}

PhantomAtom::PhantomAtom(const sptr<Atom>& el, bool w, bool h, bool d) {
  if (el == nullptr) _elements = sptrOf<RowAtom>();
  else _elements = sptrOf<RowAtom>(el);
  _w = w, _h = h, _d = d;
}

sptr<Box> PhantomAtom::createBox(Env& env) {
  auto res = _elements->createBox(env);
  float w = (_w ? res->_width : 0);
  float h = (_h ? res->_height : 0);
  float d = (_d ? res->_depth : 0);
  float s = res->_shift;
  return sptrOf<StrutBox>(w, h, d, s);
}

/************************************ BigOperatorAtom implementation ******************************/

void BigOperatorAtom::init(const sptr<Atom>& base, const sptr<Atom>& under, const sptr<Atom>& over) {
  _base = base;
  _under = under;
  _over = over;
  _limits = false;
  _limitsSet = false;
  _type = AtomType::bigOperator;
}

sptr<Box> BigOperatorAtom::changeWidth(const sptr<Box>& b, float maxWidth) {
  if (b != nullptr && abs(maxWidth - b->_width) > PREC)
    return sptrOf<HBox>(b, maxWidth, Alignment::center);
  return b;
}

sptr<Box> BigOperatorAtom::createSideSets(Env& env) {
  return StrutBox::empty();
}

sptr<Box> BigOperatorAtom::createBox(Env& env) {
  return StrutBox::empty();
}

/*********************************** SideSetsAtom implementation **********************************/

sptr<Box> SideSetsAtom::createBox(Env& env) {
  if (_base == nullptr) {
    // create a phantom to place side-sets
    auto in = sptrOf<CharAtom>(L'M', "mathnormal");
    _base = sptrOf<PhantomAtom>(in, false, true, true);
  }

  auto bb = _base->createBox(env);
  auto pa = sptrOf<PlaceholderAtom>(0.f, bb->_height, bb->_depth, bb->_shift);

  auto* l = dynamic_cast<ScriptsAtom*>(_left.get());
  auto* r = dynamic_cast<ScriptsAtom*>(_right.get());

  if (l != nullptr && l->_base == nullptr) {
    l->_base = pa;
    l->_align = Alignment::right;
  }
  if (r != nullptr && r->_base == nullptr) r->_base = pa;

  auto hb = new HBox();
  if (_left != nullptr) hb->add(_left->createBox(env));
  hb->add(bb);
  if (_right != nullptr) hb->add(_right->createBox(env));

  return sptr<Box>(hb);
}

/******************************** OverUnderDelimiter implementation *******************************/

float OverUnderDelimiter::getMaxWidth(const Box* b, const Box* del, const Box* script) {
  // TODO
  // float mx = max(b->_width, del->_height + del->_depth);
  float mx = max(b->_width, del->_width);
  if (script != nullptr) mx = max(mx, script->_width);
  return mx;
}

sptr<Box> OverUnderDelimiter::createBox(Env& env) {
  return StrutBox::empty();
}
