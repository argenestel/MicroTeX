#include "core/formula.h"

using namespace std;
using namespace tex;

map<c32, string> Formula::_symbolMappings{
  {'+', "plus"},
  {'-', "minus"},
  {'*', "ast"},
  {'/', "slash"},
  {'(', "lparen"},
  {')', "rparen"},
  {'[', "lbrack"},
  {']', "rbrack"},
  {'{', "lbrace"},
  {'}', "rbrace"},
  {',', "comma"},
  {';', "semicolon"},
  {'!', "exclam"},
  {'?', "question"},
  {'|', "vert"},
  {':', "colon"},
  {'=', "equal"},
  {'>', "greater"},
  {'<', "less"},
};

map<wstring, wstring> Formula::_predefinedTeXFormulasAsString{
  {L"qquad",      L"\\quad\\quad"},
  {L" ",          L"\\nbsp"},
  // region dots
  {L"dotsc",      L"\\ldots"},
  {L"dots",       L"\\ldots"},
  {L"dotsb",      L"\\cdots"},
  {L"dotso",      L"\\ldots"},
  {L"dotsi",      L"\\!\\cdots"},
  {L"hdots",      L"\\ldots"},
  // endregion
  // region math symbols
  {L"relbar",     L"\\mathrel{\\smash-}"},
  {L"log",        L"\\mathop{\\mathrm{log}}\\nolimits"},
  {L"lg",         L"\\mathop{\\mathrm{lg}}\\nolimits"},
  {L"ln",         L"\\mathop{\\mathrm{ln}}\\nolimits"},
  {L"ln",         L"\\mathop{\\mathrm{ln}}\\nolimits"},
  {L"lim",        L"\\mathop{\\mathrm{lim}}"},
  {L"limsup",     L"\\mathop{\\mathrm{lim\\,sup}}"},
  {L"liminf",     L"\\mathop{\\mathrm{lim\\,inf}}"},
  {L"injlim",     L"\\mathop{\\mathrm{inj\\,lim}}"},
  {L"projlim",    L"\\mathop{\\mathrm{proj\\,lim}}"},
  {L"varinjlim",  L"\\mathop{\\underrightarrow{\\mathrm{lim}}}"},
  {L"varprojlim", L"\\mathop{\\underleftarrow{\\mathrm{lim}}}"},
  {L"varliminf",  L"\\mathop{\\underline{\\mathrm{lim}}}"},
  {L"varlimsup",  L"\\mathop{\\overline{\\mathrm{lim}}}"},
  {L"sin",        L"\\mathop{\\mathrm{sin}}\\nolimits"},
  {L"arcsin",     L"\\mathop{\\mathrm{arcsin}}\\nolimits"},
  {L"sinh",       L"\\mathop{\\mathrm{sinh}}\\nolimits"},
  {L"cos",        L"\\mathop{\\mathrm{cos}}\\nolimits"},
  {L"arccos",     L"\\mathop{\\mathrm{arccos}}\\nolimits"},
  {L"cot",        L"\\mathop{\\mathrm{cot}}\\nolimits"},
  {L"arccot",     L"\\mathop{\\mathrm{arccot}}\\nolimits"},
  {L"cosh",       L"\\mathop{\\mathrm{cosh}}\\nolimits"},
  {L"tan",        L"\\mathop{\\mathrm{tan}}\\nolimits"},
  {L"arctan",     L"\\mathop{\\mathrm{arctan}}\\nolimits"},
  {L"tanh",       L"\\mathop{\\mathrm{tanh}}\\nolimits"},
  {L"coth",       L"\\mathop{\\mathrm{coth}}\\nolimits"},
  {L"sec",        L"\\mathop{\\mathrm{sec}}\\nolimits"},
  {L"arcsec",     L"\\mathop{\\mathrm{arcsec}}\\nolimits"},
  {L"arccsc",     L"\\mathop{\\mathrm{arccsc}}\\nolimits"},
  {L"sech",       L"\\mathop{\\mathrm{sech}}\\nolimits"},
  {L"csc",        L"\\mathop{\\mathrm{csc}}\\nolimits"},
  {L"csch",       L"\\mathop{\\mathrm{csch}}\\nolimits"},
  {L"max",        L"\\mathop{\\mathrm{max}}"},
  {L"min",        L"\\mathop{\\mathrm{min}}"},
  {L"sup",        L"\\mathop{\\mathrm{sup}}"},
  {L"inf",        L"\\mathop{\\mathrm{inf}}"},
  {L"arg",        L"\\mathop{\\mathrm{arg}}\\nolimits"},
  {L"ker",        L"\\mathop{\\mathrm{ker}}\\nolimits"},
  {L"dim",        L"\\mathop{\\mathrm{dim}}\\nolimits"},
  {L"hom",        L"\\mathop{\\mathrm{hom}}\\nolimits"},
  {L"det",        L"\\mathop{\\mathrm{det}}"},
  {L"exp",        L"\\mathop{\\mathrm{exp}}\\nolimits"},
  {L"Pr",         L"\\mathop{\\mathrm{Pr}}"},
  {L"gcd",        L"\\mathop{\\mathrm{gcd}}"},
  {L"deg",        L"\\mathop{\\mathrm{deg}}\\nolimits"},
  {L"bmod",       L"\\:\\mathbin{\\mathrm{mod}}\\:"},
  // endregion
  {L"arrowvert",  L"\\vert"},
  {L"Arrowvert",  L"\\Vert"},
  {L"aa",         L"\\mathring{a}"},
  {L"AA",         L"\\mathring{A}"},
  {L"ddag",       L"\\ddagger"},
  {L"dag",        L"\\dagger"},
  {L"Doteq",      L"\\doteqdot"},
  {L"doublecup",  L"\\Cup"},
  {L"doublecap",  L"\\Cap"},
  {L"llless",     L"\\lll"},
  {L"gggtr",      L"\\ggg"},
  // region Greek
  {L"Alpha",      L"Α"},
  {L"Beta",       L"Β"},
  {L"Epsilon",    L"Ε"},
  {L"Zeta",       L"Ζ"},
  {L"Eta",        L"Η"},
  {L"Iota",       L"Ι"},
  {L"Kappa",      L"Κ"},
  {L"Mu",         L"Μ"},
  {L"Nu",         L"Ν"},
  {L"Omicron",    L"Ο"},
  {L"Rho",        L"Ρ"},
  {L"Tau",        L"Τ"},
  {L"Chi",        L"Χ"},
  {L"Nabla",      L"∇"},
  {L"omicron",    L"ο"},
  // endregion
  {L"celsius",    L"\\mathord{{}^\\circ\\mathrm{C}}"},
  {L"micro",      L"\\textmu"},
  {L"marker",     L"\\kern{0.25ex}\\rule{0.5ex}{1.2ex}\\kern{0.25ex}"},
  {L"hybull",     L"\\rule[0.6ex]{1ex}{0.2ex}"},
  {L"block",      L"\\rule{1ex}{1.2ex}"},
  {L"uhblk",      L"\\rule[0.6ex]{1ex}{0.6ex}"},
  {L"lhblk",      L"\\rule{1ex}{0.6ex}"},
  {L"rVert",      L"\\Vert"},
  {L"lVert",      L"\\Vert"},
  {L"lvert",      L"\\vert"},
  {L"rvert",      L"\\vert"},
  {L"Android",    L"\\Huge{\\android}\\normalsize"},
  {L"AndroidTeX", L"\\Android\\large{\\dT\\dE\\dX}\\normalsize"},
  {L"TeX",        L"\\large{\\dT\\dE\\dX}\\normalsize"},
  {L"LaTeX",      L"\\text{L\\kern-0.25em\\raisebox{.45ex}{\\scalebox{0.7}{A}}\\kern-.15em T\\kern-.15em\\raisebox{-.4ex}{E}\\kern-.15em X}"},
  {L"IJ",         L"{I\\kern-0.065em J}"},
  {L"ij",         L"{i\\kern-0.065em j}"},
  {L"idotsint",   L"\\int\\cdots\\int"},
};
