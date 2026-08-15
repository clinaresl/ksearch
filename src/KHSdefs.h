/* 
  KHSdefs.h
  Description: Generic definitions
  ----------------------------------------------------------------------------- 

  Started on  <Mon Feb 25 17:03:20 2013 Carlos Linares Lopez>
  Last update <jueves, 21 julio 2016 10:20:00 Carlos Linares Lopez (clinares)>
  -----------------------------------------------------------------------------

  Made by Carlos Linares Lopez
  Login   <carlos.linares@uc3m.es>
*/

#include <vector>

#ifndef KHSDEFS_H_
# define KHSDEFS_H_

namespace khs {

    // Widths of various fields used for printing solutions or combinations of them
    constexpr int name_width            = 8;
    constexpr int length_width          = 8;
    constexpr int cost_width            = 8;
    constexpr int k_width               = 8;
    constexpr int h0_width              = 9;
    constexpr int expansions_width      = 14;
    constexpr int centroids_width       = 12;
    constexpr int paths_width           = 8;
    constexpr int memory_width          = 11;
    constexpr int runtime_width         = 10;
    constexpr int expansions_sec_width  = 16;
    constexpr int inconsistencies_width = 17;
    constexpr int air_width             = 10;
    constexpr int solver_width          = 12;
    constexpr int doctor_width          = 35;
    constexpr int version_width         = 35;

    // precision used for printing floating-point numbers
    constexpr int precision             = 3;    

    // the maximum width to show the start and goal states is given next
    constexpr int max_state_width = 25;
    
    // in case that colour output is used ...
    namespace ansi {

        // the following color names are taken from the list of svgnames used in
        // xcolor under LaTeX and have exactly the same combination of R, G and
        // B.
        constexpr auto AliceBlue="\033[38;2;240;248;255m";
        constexpr auto AntiqueWhite="\033[38;2;250;235;215m";
        constexpr auto Aqua="\033[38;2;0;255;255m";
        constexpr auto Aquamarine="\033[38;2;127;255;212m";
        constexpr auto Azure="\033[38;2;240;255;255m";
        constexpr auto Beige="\033[38;2;245;245;220m";
        constexpr auto Bisque="\033[38;2;255;228;196m";
        constexpr auto Black="\033[38;2;0;0;0m";
        constexpr auto BlanchedAlmond="\033[38;2;255;235;205m";
        constexpr auto Blue="\033[38;2;0;0;255m";
        constexpr auto BlueViolet="\033[38;2;138;43;226m";
        constexpr auto Brown="\033[38;2;165;42;42m";
        constexpr auto BurlyWood="\033[38;2;222;184;135m";
        constexpr auto CadetBlue="\033[38;2;95;158;160m";
        constexpr auto Chartreuse="\033[38;2;127;255;0m";
        constexpr auto Chocolate="\033[38;2;210;105;30m";
        constexpr auto Coral="\033[38;2;255;127;80m";
        constexpr auto CornflowerBlue="\033[38;2;100;149;237m";
        constexpr auto Cornsilk="\033[38;2;255;248;220m";
        constexpr auto Crimson="\033[38;2;220;20;60m";
        constexpr auto Cyan="\033[38;2;0;255;255m";
        constexpr auto DarkBlue="\033[38;2;0;0;139m";
        constexpr auto DarkCyan="\033[38;2;0;139;139m";
        constexpr auto DarkGoldenrod="\033[38;2;184;134;11m";
        constexpr auto DarkGray="\033[38;2;169;169;169m";
        constexpr auto DarkGreen="\033[38;2;0;100;0m";
        constexpr auto DarkGrey="\033[38;2;169;169;169m";
        constexpr auto DarkKhaki="\033[38;2;189;183;107m";
        constexpr auto DarkMagenta="\033[38;2;139;0;139m";
        constexpr auto DarkOliveGreen="\033[38;2;85;107;47m";
        constexpr auto DarkOrange="\033[38;2;255;140;0m";
        constexpr auto DarkOrchid="\033[38;2;153;50;204m";
        constexpr auto DarkRed="\033[38;2;139;0;0m";
        constexpr auto DarkSalmon="\033[38;2;233;150;122m";
        constexpr auto DarkSeaGreen="\033[38;2;143;188;143m";
        constexpr auto DarkSlateBlue="\033[38;2;72;61;139m";
        constexpr auto DarkSlateGray="\033[38;2;47;79;79m";
        constexpr auto DarkSlateGrey="\033[38;2;47;79;79m";
        constexpr auto DarkTurquoise="\033[38;2;0;206;209m";
        constexpr auto DarkViolet="\033[38;2;148;0;211m";
        constexpr auto DeepPink="\033[38;2;255;20;147m";
        constexpr auto DeepSkyBlue="\033[38;2;0;191;255m";
        constexpr auto DodgerBlue="\033[38;2;30;144;255m";
        constexpr auto FireBrick="\033[38;2;178;34;34m";
        constexpr auto FloralWhite="\033[38;2;255;250;240m";
        constexpr auto ForestGreen="\033[38;2;34;139;34m";
        constexpr auto Fuchsia="\033[38;2;255;0;255m";
        constexpr auto Gainsboro="\033[38;2;220;220;220m";
        constexpr auto GhostWhite="\033[38;2;248;248;255m";
        constexpr auto Gold="\033[38;2;255;215;0m";
        constexpr auto Goldenrod="\033[38;2;218;165;32m";
        constexpr auto Gray="\033[38;2;128;128;128m";
        constexpr auto Green="\033[38;2;0;128;0m";
        constexpr auto GreenYellow="\033[38;2;173;255;47m";
        constexpr auto Grey="\033[38;2;128;128;128m";
        constexpr auto Honeydew="\033[38;2;240;255;240m";
        constexpr auto HotPink="\033[38;2;255;105;180m";
        constexpr auto IndianRed="\033[38;2;205;92;92m";
        constexpr auto Indigo="\033[38;2;75;0;130m";
        constexpr auto Ivory="\033[38;2;255;255;240m";
        constexpr auto Khaki="\033[38;2;240;230;140m";
        constexpr auto Lavender="\033[38;2;230;230;250m";
        constexpr auto LavenderBlush="\033[38;2;255;240;245m";
        constexpr auto LawnGreen="\033[38;2;124;252;0m";
        constexpr auto LemonChiffon="\033[38;2;255;250;205m";
        constexpr auto LightBlue="\033[38;2;173;216;230m";
        constexpr auto LightCoral="\033[38;2;240;128;128m";
        constexpr auto LightCyan="\033[38;2;224;255;255m";
        constexpr auto LightGoldenrod="\033[38;2;238;221;130m";
        constexpr auto LightGoldenrodYellow="\033[38;2;250;250;210m";
        constexpr auto LightGray="\033[38;2;211;211;211m";
        constexpr auto LightGreen="\033[38;2;144;238;144m";
        constexpr auto LightGrey="\033[38;2;211;211;211m";
        constexpr auto LightPink="\033[38;2;255;182;193m";
        constexpr auto LightSalmon="\033[38;2;255;160;122m";
        constexpr auto LightSeaGreen="\033[38;2;32;178;170m";
        constexpr auto LightSkyBlue="\033[38;2;135;206;250m";
        constexpr auto LightSlateBlue="\033[38;2;132;112;255m";
        constexpr auto LightSlateGray="\033[38;2;119;136;153m";
        constexpr auto LightSlateGrey="\033[38;2;119;136;153m";
        constexpr auto LightSteelBlue="\033[38;2;176;196;222m";
        constexpr auto LightYellow="\033[38;2;255;255;224m";
        constexpr auto Lime="\033[38;2;0;255;0m";
        constexpr auto LimeGreen="\033[38;2;50;205;50m";
        constexpr auto Linen="\033[38;2;250;240;230m";
        constexpr auto Magenta="\033[38;2;255;0;255m";
        constexpr auto Maroon="\033[38;2;128;0;0m";
        constexpr auto MediumAquamarine="\033[38;2;102;205;170m";
        constexpr auto MediumBlue="\033[38;2;0;0;205m";
        constexpr auto MediumOrchid="\033[38;2;186;85;211m";
        constexpr auto MediumPurple="\033[38;2;147;112;219m";
        constexpr auto MediumSeaGreen="\033[38;2;60;179;113m";
        constexpr auto MediumSlateBlue="\033[38;2;123;104;238m";
        constexpr auto MediumSpringGreen="\033[38;2;0;250;154m";
        constexpr auto MediumTurquoise="\033[38;2;72;209;204m";
        constexpr auto MediumVioletRed="\033[38;2;199;21;133m";
        constexpr auto MidnightBlue="\033[38;2;25;25;112m";
        constexpr auto MintCream="\033[38;2;245;255;250m";
        constexpr auto MistyRose="\033[38;2;255;228;225m";
        constexpr auto Moccasin="\033[38;2;255;228;181m";
        constexpr auto NavajoWhite="\033[38;2;255;222;173m";
        constexpr auto Navy="\033[38;2;0;0;128m";
        constexpr auto NavyBlue="\033[38;2;0;0;128m";
        constexpr auto OldLace="\033[38;2;253;245;230m";
        constexpr auto Olive="\033[38;2;128;128;0m";
        constexpr auto OliveDrab="\033[38;2;107;142;35m";
        constexpr auto Orange="\033[38;2;255;165;0m";
        constexpr auto OrangeRed="\033[38;2;255;69;0m";
        constexpr auto Orchid="\033[38;2;218;112;214m";
        constexpr auto PaleGoldenrod="\033[38;2;238;232;170m";
        constexpr auto PaleGreen="\033[38;2;152;251;152m";
        constexpr auto PaleTurquoise="\033[38;2;175;238;238m";
        constexpr auto PaleVioletRed="\033[38;2;219;112;147m";
        constexpr auto PapayaWhip="\033[38;2;255;239;213m";
        constexpr auto PeachPuff="\033[38;2;255;218;185m";
        constexpr auto Peru="\033[38;2;205;133;63m";
        constexpr auto Pink="\033[38;2;255;192;203m";
        constexpr auto Plum="\033[38;2;221;160;221m";
        constexpr auto PowderBlue="\033[38;2;176;224;230m";
        constexpr auto Purple="\033[38;2;128;0;128m";
        constexpr auto Red="\033[38;2;255;0;0m";
        constexpr auto RosyBrown="\033[38;2;188;143;143m";
        constexpr auto RoyalBlue="\033[38;2;65;105;225m";
        constexpr auto SaddleBrown="\033[38;2;139;69;19m";
        constexpr auto Salmon="\033[38;2;250;128;114m";
        constexpr auto SandyBrown="\033[38;2;244;164;96m";
        constexpr auto SeaGreen="\033[38;2;46;139;87m";
        constexpr auto Seashell="\033[38;2;255;245;238m";
        constexpr auto Sienna="\033[38;2;160;82;45m";
        constexpr auto Silver="\033[38;2;192;192;192m";
        constexpr auto SkyBlue="\033[38;2;135;206;235m";
        constexpr auto SlateBlue="\033[38;2;106;90;205m";
        constexpr auto SlateGray="\033[38;2;112;128;144m";
        constexpr auto SlateGrey="\033[38;2;112;128;144m";
        constexpr auto Snow="\033[38;2;255;250;250m";
        constexpr auto SpringGreen="\033[38;2;0;255;127m";
        constexpr auto SteelBlue="\033[38;2;70;130;180m";
        constexpr auto Tan="\033[38;2;210;180;140m";
        constexpr auto Teal="\033[38;2;0;128;128m";
        constexpr auto Thistle="\033[38;2;216;191;216m";
        constexpr auto Tomato="\033[38;2;255;99;71m";
        constexpr auto Turquoise="\033[38;2;64;224;208m";
        constexpr auto Violet="\033[38;2;238;130;238m";
        constexpr auto VioletRed="\033[38;2;208;32;144m";
        constexpr auto Wheat="\033[38;2;245;222;179m";
        constexpr auto White="\033[38;2;255;255;255m";
        constexpr auto WhiteSmoke="\033[38;2;245;245;245m";
        constexpr auto Yellow="\033[38;2;255;255;0m";
        constexpr auto YellowGreen="\033[38;2;154;205;50m";

        // and also dimmed versions
        constexpr auto DimAliceBlue="\033[2;38;2;240;248;255m";
        constexpr auto DimAntiqueWhite="\033[2;38;2;250;235;215m";
        constexpr auto DimAqua="\033[2;38;2;0;255;255m";
        constexpr auto DimAquamarine="\033[2;38;2;127;255;212m";
        constexpr auto DimAzure="\033[2;38;2;240;255;255m";
        constexpr auto DimBeige="\033[2;38;2;245;245;220m";
        constexpr auto DimBisque="\033[2;38;2;255;228;196m";
        constexpr auto DimBlack="\033[2;38;2;0;0;0m";
        constexpr auto DimBlanchedAlmond="\033[2;38;2;255;235;205m";
        constexpr auto DimBlue="\033[2;38;2;0;0;255m";
        constexpr auto DimBlueViolet="\033[2;38;2;138;43;226m";
        constexpr auto DimBrown="\033[2;38;2;165;42;42m";
        constexpr auto DimBurlyWood="\033[2;38;2;222;184;135m";
        constexpr auto DimCadetBlue="\033[2;38;2;95;158;160m";
        constexpr auto DimChartreuse="\033[2;38;2;127;255;0m";
        constexpr auto DimChocolate="\033[2;38;2;210;105;30m";
        constexpr auto DimCoral="\033[2;38;2;255;127;80m";
        constexpr auto DimCornflowerBlue="\033[2;38;2;100;149;237m";
        constexpr auto DimCornsilk="\033[2;38;2;255;248;220m";
        constexpr auto DimCrimson="\033[2;38;2;220;20;60m";
        constexpr auto DimCyan="\033[2;38;2;0;255;255m";
        constexpr auto DimDarkBlue="\033[2;38;2;0;0;139m";
        constexpr auto DimDarkCyan="\033[2;38;2;0;139;139m";
        constexpr auto DimDarkGoldenrod="\033[2;38;2;184;134;11m";
        constexpr auto DimDarkGray="\033[2;38;2;169;169;169m";
        constexpr auto DimDarkGreen="\033[2;38;2;0;100;0m";
        constexpr auto DimDarkGrey="\033[2;38;2;169;169;169m";
        constexpr auto DimDarkKhaki="\033[2;38;2;189;183;107m";
        constexpr auto DimDarkMagenta="\033[2;38;2;139;0;139m";
        constexpr auto DimDarkOliveGreen="\033[2;38;2;85;107;47m";
        constexpr auto DimDarkOrange="\033[2;38;2;255;140;0m";
        constexpr auto DimDarkOrchid="\033[2;38;2;153;50;204m";
        constexpr auto DimDarkRed="\033[2;38;2;139;0;0m";
        constexpr auto DimDarkSalmon="\033[2;38;2;233;150;122m";
        constexpr auto DimDarkSeaGreen="\033[2;38;2;143;188;143m";
        constexpr auto DimDarkSlateBlue="\033[2;38;2;72;61;139m";
        constexpr auto DimDarkSlateGray="\033[2;38;2;47;79;79m";
        constexpr auto DimDarkSlateGrey="\033[2;38;2;47;79;79m";
        constexpr auto DimDarkTurquoise="\033[2;38;2;0;206;209m";
        constexpr auto DimDarkViolet="\033[2;38;2;148;0;211m";
        constexpr auto DimDeepPink="\033[2;38;2;255;20;147m";
        constexpr auto DimDeepSkyBlue="\033[2;38;2;0;191;255m";
        constexpr auto DimGray="\033[38;2;105;105;105m";
        constexpr auto DimDimGray="\033[2;38;2;105;105;105m";
        constexpr auto DimGrey="\033[38;2;105;105;105m";
        constexpr auto DimDimGrey="\033[2;38;2;105;105;105m";
        constexpr auto DimDodgerBlue="\033[2;38;2;30;144;255m";
        constexpr auto DimFireBrick="\033[2;38;2;178;34;34m";
        constexpr auto DimFloralWhite="\033[2;38;2;255;250;240m";
        constexpr auto DimForestGreen="\033[2;38;2;34;139;34m";
        constexpr auto DimFuchsia="\033[2;38;2;255;0;255m";
        constexpr auto DimGainsboro="\033[2;38;2;220;220;220m";
        constexpr auto DimGhostWhite="\033[2;38;2;248;248;255m";
        constexpr auto DimGold="\033[2;38;2;255;215;0m";
        constexpr auto DimGoldenrod="\033[2;38;2;218;165;32m";
        constexpr auto DimGrayAnsi="\033[2;38;2;128;128;128m";
        constexpr auto DimGreen="\033[2;38;2;0;128;0m";
        constexpr auto DimGreenYellow="\033[2;38;2;173;255;47m";
        constexpr auto DimGreyAnsi="\033[2;38;2;128;128;128m";
        constexpr auto DimHoneydew="\033[2;38;2;240;255;240m";
        constexpr auto DimHotPink="\033[2;38;2;255;105;180m";
        constexpr auto DimIndianRed="\033[2;38;2;205;92;92m";
        constexpr auto DimIndigo="\033[2;38;2;75;0;130m";
        constexpr auto DimIvory="\033[2;38;2;255;255;240m";
        constexpr auto DimKhaki="\033[2;38;2;240;230;140m";
        constexpr auto DimLavender="\033[2;38;2;230;230;250m";
        constexpr auto DimLavenderBlush="\033[2;38;2;255;240;245m";
        constexpr auto DimLawnGreen="\033[2;38;2;124;252;0m";
        constexpr auto DimLemonChiffon="\033[2;38;2;255;250;205m";
        constexpr auto DimLightBlue="\033[2;38;2;173;216;230m";
        constexpr auto DimLightCoral="\033[2;38;2;240;128;128m";
        constexpr auto DimLightCyan="\033[2;38;2;224;255;255m";
        constexpr auto DimLightGoldenrod="\033[2;38;2;238;221;130m";
        constexpr auto DimLightGoldenrodYellow="\033[2;38;2;250;250;210m";
        constexpr auto DimLightGray="\033[2;38;2;211;211;211m";
        constexpr auto DimLightGreen="\033[2;38;2;144;238;144m";
        constexpr auto DimLightGrey="\033[2;38;2;211;211;211m";
        constexpr auto DimLightPink="\033[2;38;2;255;182;193m";
        constexpr auto DimLightSalmon="\033[2;38;2;255;160;122m";
        constexpr auto DimLightSeaGreen="\033[2;38;2;32;178;170m";
        constexpr auto DimLightSkyBlue="\033[2;38;2;135;206;250m";
        constexpr auto DimLightSlateBlue="\033[2;38;2;132;112;255m";
        constexpr auto DimLightSlateGray="\033[2;38;2;119;136;153m";
        constexpr auto DimLightSlateGrey="\033[2;38;2;119;136;153m";
        constexpr auto DimLightSteelBlue="\033[2;38;2;176;196;222m";
        constexpr auto DimLightYellow="\033[2;38;2;255;255;224m";
        constexpr auto DimLime="\033[2;38;2;0;255;0m";
        constexpr auto DimLimeGreen="\033[2;38;2;50;205;50m";
        constexpr auto DimLinen="\033[2;38;2;250;240;230m";
        constexpr auto DimMagenta="\033[2;38;2;255;0;255m";
        constexpr auto DimMaroon="\033[2;38;2;128;0;0m";
        constexpr auto DimMediumAquamarine="\033[2;38;2;102;205;170m";
        constexpr auto DimMediumBlue="\033[2;38;2;0;0;205m";
        constexpr auto DimMediumOrchid="\033[2;38;2;186;85;211m";
        constexpr auto DimMediumPurple="\033[2;38;2;147;112;219m";
        constexpr auto DimMediumSeaGreen="\033[2;38;2;60;179;113m";
        constexpr auto DimMediumSlateBlue="\033[2;38;2;123;104;238m";
        constexpr auto DimMediumSpringGreen="\033[2;38;2;0;250;154m";
        constexpr auto DimMediumTurquoise="\033[2;38;2;72;209;204m";
        constexpr auto DimMediumVioletRed="\033[2;38;2;199;21;133m";
        constexpr auto DimMidnightBlue="\033[2;38;2;25;25;112m";
        constexpr auto DimMintCream="\033[2;38;2;245;255;250m";
        constexpr auto DimMistyRose="\033[2;38;2;255;228;225m";
        constexpr auto DimMoccasin="\033[2;38;2;255;228;181m";
        constexpr auto DimNavajoWhite="\033[2;38;2;255;222;173m";
        constexpr auto DimNavy="\033[2;38;2;0;0;128m";
        constexpr auto DimNavyBlue="\033[2;38;2;0;0;128m";
        constexpr auto DimOldLace="\033[2;38;2;253;245;230m";
        constexpr auto DimOlive="\033[2;38;2;128;128;0m";
        constexpr auto DimOliveDrab="\033[2;38;2;107;142;35m";
        constexpr auto DimOrange="\033[2;38;2;255;165;0m";
        constexpr auto DimOrangeRed="\033[2;38;2;255;69;0m";
        constexpr auto DimOrchid="\033[2;38;2;218;112;214m";
        constexpr auto DimPaleGoldenrod="\033[2;38;2;238;232;170m";
        constexpr auto DimPaleGreen="\033[2;38;2;152;251;152m";
        constexpr auto DimPaleTurquoise="\033[2;38;2;175;238;238m";
        constexpr auto DimPaleVioletRed="\033[2;38;2;219;112;147m";
        constexpr auto DimPapayaWhip="\033[2;38;2;255;239;213m";
        constexpr auto DimPeachPuff="\033[2;38;2;255;218;185m";
        constexpr auto DimPeru="\033[2;38;2;205;133;63m";
        constexpr auto DimPink="\033[2;38;2;255;192;203m";
        constexpr auto DimPlum="\033[2;38;2;221;160;221m";
        constexpr auto DimPowderBlue="\033[2;38;2;176;224;230m";
        constexpr auto DimPurple="\033[2;38;2;128;0;128m";
        constexpr auto DimRed="\033[2;38;2;255;0;0m";
        constexpr auto DimRosyBrown="\033[2;38;2;188;143;143m";
        constexpr auto DimRoyalBlue="\033[2;38;2;65;105;225m";
        constexpr auto DimSaddleBrown="\033[2;38;2;139;69;19m";
        constexpr auto DimSalmon="\033[2;38;2;250;128;114m";
        constexpr auto DimSandyBrown="\033[2;38;2;244;164;96m";
        constexpr auto DimSeaGreen="\033[2;38;2;46;139;87m";
        constexpr auto DimSeashell="\033[2;38;2;255;245;238m";
        constexpr auto DimSienna="\033[2;38;2;160;82;45m";
        constexpr auto DimSilver="\033[2;38;2;192;192;192m";
        constexpr auto DimSkyBlue="\033[2;38;2;135;206;235m";
        constexpr auto DimSlateBlue="\033[2;38;2;106;90;205m";
        constexpr auto DimSlateGray="\033[2;38;2;112;128;144m";
        constexpr auto DimSlateGrey="\033[2;38;2;112;128;144m";
        constexpr auto DimSnow="\033[2;38;2;255;250;250m";
        constexpr auto DimSpringGreen="\033[2;38;2;0;255;127m";
        constexpr auto DimSteelBlue="\033[2;38;2;70;130;180m";
        constexpr auto DimTan="\033[2;38;2;210;180;140m";
        constexpr auto DimTeal="\033[2;38;2;0;128;128m";
        constexpr auto DimThistle="\033[2;38;2;216;191;216m";
        constexpr auto DimTomato="\033[2;38;2;255;99;71m";
        constexpr auto DimTurquoise="\033[2;38;2;64;224;208m";
        constexpr auto DimViolet="\033[2;38;2;238;130;238m";
        constexpr auto DimVioletRed="\033[2;38;2;208;32;144m";
        constexpr auto DimWheat="\033[2;38;2;245;222;179m";
        constexpr auto DimWhite="\033[2;38;2;255;255;255m";
        constexpr auto DimWhiteSmoke="\033[2;38;2;245;245;245m";
        constexpr auto DimYellow="\033[2;38;2;255;255;0m";
        constexpr auto DimYellowGreen="\033[2;38;2;154;205;50m";

        // and finally, the ANSI escape characters to reset
        constexpr auto reset="\033[0m";
    }
    
    // template functions
  
    // return a forward iterator to the first occurrence within the given range
    // which does NOT verify the given binary predicate
    template<class ForwardIt, class BinaryPredicate>
    ForwardIt adjacent_find_not (ForwardIt first, ForwardIt last, BinaryPredicate p) {

        if (first == last) {
            return last;
        }
        ForwardIt next = first;
        ++next;
        for (; next != last; ++next, ++first) {
            if (!p(*first, *next)) {
                return first;
            }
        }
        return last;
    }

    // the following service returns true if two vectors are the same and false
    // otherwise
    template<class T> 
    extern bool duplicate (const std::vector<T>& path1, const std::vector<T>& path2)
    {

        // first, if they have different lengths, then they are different
        if (path1.size () != path2.size ())
            return false;

        // second, traverse all the elements of each path
        for (unsigned int idx=0 ; idx < path1.size () ; idx++)

            // if they differ in one item, then they are different
            if ( !(path1[idx] == path2[idx]) )
                return false;

        // if this point was reached, then both paths are the same
        return true;
    }

} // namespace khs

#endif /* !KHSDEFS_H_ */



/* Local Variables: */
/* mode:c++ */
/* fill-column:80 */
/* End: */
