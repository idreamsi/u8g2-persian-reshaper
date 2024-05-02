/*
  ScrollingText.ino
  This will scroll text on the display. 
  Enable U8g2 16 bit mode (see FAQ) for larger text!
*/

#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 12, /* dc=*/ 4, /* reset=*/ 6);  // Arduboy (Production, Kickstarter Edition)

int xPos, yPos, xxPos, yyPos;
char txt[] = "سلام علیکم";
String msg;

//----------------------------------------------------------------------------
//#define DEBUG
#define LCDHeight                       64
#define TXTWidth(t)                     u8g2.getUTF8Width(t)
#define LCDWidth                        u8g2.getDisplayWidth()
#define ALINE_CENTER(t)                 ((LCDWidth - (u8g2.getUTF8Width(t))) / 2)
#define ALINE_RIGHT(t)                  (LCDWidth - u8g2.getUTF8Width(t))
#define ALINE_LEFT                      0
#define N_DISTINCT_CHARACTERS           62
#define IS_UNICODE(c)                   (((c) & 0xc0) == 0xc0)
#define VERSION                         1
//----------------------------------------------------------------------------
typedef struct prGlyph {
  int AsciiCode;
  char* codeGlyph;
  char* isoGlyph;
  char* iniGlyph;
  char* midGlyph;
  char* endGlyph;
};

const prGlyph prForms[] PROGMEM = {
    // Ascii Code, Code, Isolated, Initial, Medial, Final
    {193, "\u0621", "\uFE80", "\uFE80", "\uFE80", "\uFE80" },     //1 HAMZA ء [*]
    {194, "\u0622", "\uFE81", "\uFE81", "\uFE82", "\uFE82" },     //2 ALEF_MADDA آ [*]
    {195, "\u0623", "\uFE83", "\uFE83", "\uFE84", "\uFE84" },     //3 ALEF_HAMZA_ABOVE أ [*]
    {196, "\u0624", "\uFE85", "\uFE85", "\uFE86", "\uFE86" },     //4 WAW_HAMZA ؤ [*]
    {197, "\u0625", "\uFE87", "\uFE87", "\uFE88", "\uFE88" },     //5 ALEF_HAMZA_BELOW إ [*]
    {198, "\u0626", "\uFE89", "\uFE8B", "\uFE8C", "\uFE8A" },     //6 YEH_HAMZA ئ [*]
    {199, "\u0627", "\uFE8D", "\uFE8D", "\uFE8E", "\uFE8E" },     //7 ALEF ا [*]
    {200, "\u0628", "\uFE8F", "\uFE91", "\uFE92", "\uFE90" },     //8 BEH ب
    {555, "\u0629", "\uFE93", "\uFE93", "\uFE94", "\uFE94" },     //9 TEH_MARBUTA ة [*]
    {202, "\u062A", "\uFE95", "\uFE97", "\uFE98", "\uFE96" },     //10 TEH ت
    {203, "\u062B", "\uFE99", "\uFE9B", "\uFE9C", "\uFE9A" },     //11 THEH ث
    {204, "\u062C", "\uFE9D", "\uFE9F", "\uFEA0", "\uFE9E" },     //12 JEEM ج
    {205, "\u062D", "\uFEA1", "\uFEA3", "\uFEA4", "\uFEA2" },     //13 HAH ح
    {206, "\u062E", "\uFEA5", "\uFEA7", "\uFEA8", "\uFEA6" },     //14 KHAH خ
    {207, "\u062F", "\uFEA9", "\uFEA9", "\uFEAA", "\uFEAA" },     //15 DAL د [*]
    {208, "\u0630", "\uFEAB", "\uFEAB", "\uFEAC", "\uFEAC" },     //16 THAL ذ [*]
    {209, "\u0631", "\uFEAD", "\uFEAD", "\uFEAE", "\uFEAE" },     //17 REH ر [*]
    {210, "\u0632", "\uFEAF", "\uFEAF", "\uFEB0", "\uFEB0" },     //18 ZAIN ز [*]
    {184, "\u0698", "\uFB8A", "\uFB8A", "\uFB8B", "\uFB8B" },     //19 ZHEH ژ [*]
    {211, "\u0633", "\uFEB1", "\uFEB3", "\uFEB4", "\uFEB2" },     //20 SEEN 
    {212, "\u0634", "\uFEB5", "\uFEB7", "\uFEB8", "\uFEB6" },     //21 SHEEN 
    {213, "\u0635", "\uFEB9", "\uFEBB", "\uFEBC", "\uFEBA" },     //22 SAD ص
    {214, "\u0636", "\uFEBD", "\uFEBF", "\uFEC0", "\uFEBE" },     //23 DAD ض
    {215, "\u0637", "\uFEC1", "\uFEC3", "\uFEC4", "\uFEC2" },     //24 TAH ط
    {216, "\u0638", "\uFEC5", "\uFEC7", "\uFEC8", "\uFEC6" },     //25 ZAH ظ
    {217, "\u0639", "\uFEC9", "\uFECB", "\uFECC", "\uFECA" },     //26 AIN ع
    {218, "\u063A", "\uFECD", "\uFECF", "\uFED0", "\uFECE" },     //27 GHAIN غ
    {160, "\u0640", "\u0640", "\u0640", "\u0640", "\u0640" },     //28 TATWEEL ـ
    {161, "\u0641", "\uFED1", "\uFED3", "\uFED4", "\uFED2" },     //29 FEH ف
    {162, "\u0642", "\uFED5", "\uFED7", "\uFED8", "\uFED6" },     //30 QAF ق
    {163, "\u0643", "\uFED9", "\uFEDB", "\uFEDC", "\uFEDA" },     //31 KAF Arabic ك
    {164, "\u0644", "\uFEDD", "\uFEDF", "\uFEE0", "\uFEDE" },     //32 LAM ل
    {165, "\u0645", "\uFEE1", "\uFEE3", "\uFEE4", "\uFEE2" },     //33 MEEM م
    {228, "\u0646", "\uFEE5", "\uFEE7", "\uFEE8", "\uFEE6" },     //34 NOON ن
    {167, "\u0647", "\uFEE9", "\uFEEB", "\uFEEC", "\uFEEA" },     //35 HEH ه
    {168, "\u0648", "\uFEED", "\uFEED", "\uFEEE", "\uFEEE" },     //36 WAW و [*]
    {169, "\u0649", "\uFEEF", "\uFEEF", "\uFEF0", "\uFEF0" },     //37 ALEF_MAKSURA [*]
    {170, "\u064A", "\uFEF1", "\uFEF3", "\uFEF4", "\uFEF2" },     //38 YEH Arabic ي 
    {172, "\u06CC", "\uFBFC", "\uFBFE", "\uFBFF", "\uFBFD" },     //39 YEH Farsi ی
    {141, "\u0686", "\uFB7A", "\uFB7C", "\uFB7D", "\uFB7B" },     //40 CHEH چ
    {222, "\u067E", "\uFB56", "\uFB58", "\uFB59", "\uFB57" },     //41 Peh پ
    {144, "\u06AF", "\uFB92", "\uFB94", "\uFB95", "\uFB93" },     //42 Gaf گ
    {201, "\u06A9", "\uFB8E", "\uFB90", "\uFB91", "\uFB8F" },     //43 Kaf ک
    {32, "\u0020", "\u0020", "\u0020", "\u0020", "\u0020" },      //44 Space
    {44, "\u060C", "\u060C", "\u060C", "\u060C", "\u060C" },    //45 Kama
    {20, "\u200C", "\u200C", "\u200C", "\u200C","\u200C" },     //46 half-space
    {58, "\u003A", "\u003A", "\u003A", "\u003A", "\u003A" },      //47 :
    {187, "\u061B", "\u061B", "\u061B", "\u061B", "\u061B" },     //48 ؛
    {46, "\u002E", "\u002E", "\u002E", "\u002E", "\u002E" },      //49 .
    {191, "\u061F", "\u061F", "\u061F", "\u061F", "\u061F" },     //50 ؟
    {48, "\u06F0", "\u06F0", "\u06F0", "\u06F0", "\u06F0" },      //51 0
    {49, "\u06F1", "\u06F1", "\u06F1", "\u06F1", "\u06F1" },      //52 1
    {50, "\u06F2", "\u06F2", "\u06F2", "\u06F2", "\u06F2" },      //53 2
    {51, "\u06F3", "\u06F3", "\u06F3", "\u06F3", "\u06F3" },      //54 3
    {52, "\u06F4", "\u06F4", "\u06F4", "\u06F4", "\u06F4" },      //55 4
    {53, "\u06F5", "\u06F5", "\u06F5", "\u06F5", "\u06F5" },      //56 5
    {54, "\u06F6", "\u06F6", "\u06F6", "\u06F6", "\u06F6" },      //57 6
    {55, "\u06F7", "\u06F7", "\u06F7", "\u06F7", "\u06F7" },      //58 7
    {56, "\u06F8", "\u06F8", "\u06F8", "\u06F8", "\u06F8" },      //59 8
    {57, "\u06F9", "\u06F9", "\u06F9", "\u06F9", "\u06F9" },      //60 9
    {41, "\u0028", "\u0028", "\u0028", "\u0028", "\u0028" },      //61 (  
    {40, "\u0029", "\u0029", "\u0029", "\u0029", "\u0029" }       //62 )
};
//----------------------------------------------------------------------------
bool isFromTheSet1(unsigned char ch){
    const unsigned char theSet1[18] = {
        32, '\0', 199, 194, 207, 208, 209, 210,
        184, 168, 191, 40, 41, 46, 33, 44, 58, 248};
    int i = 0;
    while (i < 18)
    {
        if(ch == theSet1[i])
            return true;        
        ++i;
    }
    return false;
}
//----------------------------------------------------------------------------
bool isFromTheSet2(unsigned char ch){
    const unsigned char theSet1[10] = {
        32, '\0', 191, 40, 41, 46, 33, 44,
        58, 248 };
    int i = 0;
    while (i < 10)
    {
        if(ch == theSet1[i])
            return true;        
        ++i;
    }
    return false;
}
//----------------------------------------------------------------------------
int FindGlyph(unsigned char chFind){
  for (int i = 0; i < N_DISTINCT_CHARACTERS; i++) {
    if (pgm_read_word(&(prForms[i].AsciiCode)) == chFind) {
  return i;
  break;
    }
  }
  return -1;
}
//----------------------------------------------------------------------------
String prReshaper(char *Text){
  
  String prBuffer = "";
  int stat = 0;
  unsigned char pLetter = ' ';      //Previous word
  unsigned char letter;               //Letter
  unsigned char nLetter;              //Next word
  unsigned char temp;
  
  while(temp = *Text++){
    //is Number ?
    if (temp >= '0' && temp <= '9') {
      //d = temp - '0';
      letter = temp;
    }
    else if(temp >= 128){ 
      letter = *Text++;
      letter += 32;
      temp += 32;
      if(letter == 207){
        if(temp == 218 || temp == 250){ 
          letter = 144; //گ
        }
      }
      else if(letter == 166)
        { 
        if(temp == 218 || temp == 250){ //چ
          letter = 141;
        }
        else
        { 
          letter = 228; //ن
        }
      }
    } 
    else
    {
      letter = temp;
    }
  //
    if(letter == 172)
    {
      if(temp == 248 || temp == 32)
      {
        letter = 44;
      }
    }        
    temp = *Text++;
    if(temp >= 128)
    {
      nLetter = *Text++; 
      nLetter += 32; 
      temp += 32;
      if(nLetter == 207)
      {
        if(temp == 218 || temp == 250)
        { 
          nLetter = 144; //گ
        }
      }
    else if(nLetter == 166)
    {
      if(temp == 218 || temp == 250)
      { //چ
        nLetter = 141; 
      }
      else
      {
        nLetter = 228; //ن
      }
    }
    *Text--;
    *Text--;
    }
    else
    {
      nLetter = temp; 
      *Text--;
    }
  //
    if(nLetter == 172)
    {
      if(temp == 248 || temp == 32)
      {
        nLetter = 44;
      }
    }      
  int isunk = 0; 
  /*
  Final: at the end of the word.
  Medial: at the middle of the word.
  Initial: at the beginning of the word.
  Isolated: the character alone (not part of a word).
  */
    if (isFromTheSet1(pLetter))
      if (isFromTheSet2(nLetter))
        stat = 0;  //Isolated
      else
        stat = 1;  //Initial
    else
      if (isFromTheSet2(nLetter))
        stat = 2;  //Final
      else
        stat = 3;  //Medial

  int number = FindGlyph(letter);
  
  #ifdef DEBUG
    Serial.print("Letter code: ");
    Serial.println(letter);
    Serial.print("Number is: ");
    Serial.println(number);
    Serial.print("Pos: ");
    Serial.println(stat);
    Serial.println("--------------");
  #endif

	switch (stat){
		case 0: //Isolated
			//prBuffer += (char*)pgm_read_word(&(prForms[number].isoGlyph));
			prBuffer += (char*)(prForms[number].isoGlyph);
			break;
		case 1: //Initial
			//prBuffer += (char*)pgm_read_word(&(prForms[number].iniGlyph));
			prBuffer += (char*)(prForms[number].iniGlyph);
			break;
		case 2: //Final
			//prBuffer += (char*)pgm_read_word(&(prForms[number].endGlyph));
			prBuffer += (char*)(prForms[number].endGlyph);
			break;
		case 3: //Medial
			//prBuffer += (char*)pgm_read_word(&(prForms[number].midGlyph));
			prBuffer += (char*)(prForms[number].midGlyph);
			break;		
		default:
			isunk = 1;
			break;	
	}  
    if(isunk == 0)
      pLetter = letter;  
  }
  //utf8rev(prBuffer.c_str());
  //https://stackoverflow.com/questions/20984220/invalid-conversion-from-const-char-to-char
  utf8rev((char *)(prBuffer.c_str()));
  
  return prBuffer; 
}
//----------------------------------------------------------------------------
// https://stackoverflow.com/questions/199260/how-do-i-reverse-a-utf-8-string-in-place
void utf8rev(char *str){
    /* this assumes that str is valid UTF-8 */
    char    *scanl, *scanr, *scanr2, c;
    /* first reverse the string */
    for (scanl = str, scanr = str + strlen(str); scanl < scanr;)
        c = *scanl, *scanl++= *--scanr, *scanr= c;
    /* then scan all bytes and reverse each multibyte character */
    for (scanl = scanr = str; c = *scanr++;) {
        if ( (c & 0x80) == 0) // ASCII char
            scanl = scanr;
        else if ( (c & 0xc0) == 0xc0 ) { // start of multibyte
            scanr2 = scanr;
            switch (scanr - scanl) {
                case 4: c = *scanl, *scanl++= *--scanr, *scanr = c; // fallthrough
                case 3: // fallthrough
                case 2: c = *scanl, *scanl++= *--scanr, *scanr = c;
            }
            scanr = scanl = scanr2;
        }
    }
}
//----------------------------------------------------------------------------
//UTF-8 strlen function
int strlen_utf8(char *s) {
   int i = 0, j = 0;
   while (s[i]) {
     if ((s[i] & 0xc0) != 0x80) j++;
     i++;
   }
   return j;
}


// This example shows a scrolling text.
// If U8G2_16BIT is not set (default), then the pixel width of the text must be lesser than 128
// If U8G2_16BIT is set, then the pixel width an be up to 32000 


void draw(void)
{
    u8g2.drawUTF8(xPos, 20, msg.c_str());
    u8g2.drawUTF8(LCDWidth - xxPos, 50, msg.c_str());
    //u8g2.drawUTF8(ALINE_CENTER(txt), yPos, msg.c_str());
    //u8g2.drawUTF8(ALINE_CENTER(txt), yyPos, msg.c_str());
}

void setup(void)
{  
    u8g2.begin();
    //Set font
    //u8g2.setFont(u8g2_font_samim_12_t_all);                 //Number view in both modes. (English & Persian)
    //u8g2.setFont(u8g2_font_iranian_sans_16_t_all);          //Number view in both modes. (English & Persian)
    //u8g2.setFont(u8g2_font_ganj_nameh_sans16_t_all);        //No number view.
    //u8g2.setFont(u8g2_font_samim_fd_16_t_all);              //By default, this font displays numbers in Persian.
    u8g2.setFont(u8g2_font_iranian_sans_14_t_all);
    u8g2.enableUTF8Print();
    //u8g2.setColorIndex(0);                                   // BW display
    u8g2.setFontMode(0);                                       // enable transparent mode, which is faster
    xPos = -TXTWidth(txt);
    yyPos = LCDHeight + 20;
    msg = prReshaper(txt);
}

void loop(void)
{
    // picture loop
    u8g2.firstPage();
    do {
        draw();
    } while (u8g2.nextPage());
    if (++xPos >= LCDWidth) xPos = -TXTWidth(txt);            //Left to Right
    if (++xxPos >= LCDWidth + TXTWidth(txt)) xxPos = 0;       //Right to Left
    if (++yPos >= TXTWidth(txt)) yPos = 0;                    //UP to Down
    if (--yyPos <= 0) yyPos = LCDHeight + 20;                    //Down to UP
    delay(5);
}
