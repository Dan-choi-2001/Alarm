#include <Wire.h>
#include<EEPROM.h>
#include <RTClib.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);  //dinh nghia cac chan LCD
RTC_DS1307 RTC;
int tmp, Inc, hor, mIn, addr = 11;
int set = 8;
int cge = 9;
int mod = 10;
int off = 0;
#define buz 11
int Hor, Min, Sec;

//////////chinh thoi gian//////////
void time() {
    int tmp = 1, mins = 0, hors = 0;//, secs = 0;
    //bam nut so 3
    while (tmp == 1) {
        off = 0;
        if (digitalRead(cge) == 0) {
            Hor++;  //neu an nut so 2 thi se tang 1 don vi
            if (Hor == 24) {
                Hor = 0;
            }
        }
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Set Alarm Time:");

        lcd.setCursor(0, 1);
        if (Hor <= 9)
            lcd.print("0"); //thuc chat man hinh ko in ra so 0 neu gio hoac phut nho hon 10
        lcd.print(Hor);
        lcd.print(":");
        if (Min <= 9)
            lcd.print("0");
        lcd.print(Min);
        //lcd.print(":");
        //lcd.print(Sec);
        delay(200);

        //hieu ung nhay nhay
        lcd.setCursor(0, 1);
        lcd.print("  ");
        lcd.print(":");
        if (Min <= 9)
            lcd.print("0");
        lcd.print(Min);
        //lcd.print(":");
        //lcd.print(Sec);
        delay(200);
        if (digitalRead(set) == 0) {  //sau khi set xong an nut so 3 de luu
            hor = Hor;
            EEPROM.write(addr++, hor);
            tmp = 2;
            while (digitalRead(set) == 0);
        }
    }

    //bam nut so 3 lan nua
    while (tmp == 2) {
        if (digitalRead(cge) == 0) {
            Min++;
            if (Min == 60) {
                Min = 0;
            }
        }
        lcd.setCursor(0, 1);
        lcd.print(Hor);
        if (Hor <=9)
            lcd.print("0");
        lcd.print(":");
        if (Min <= 9)
            lcd.print("0");
        lcd.print(Min);
        //lcd.print(":");
        //lcd.print(Sec);

        //hieu ung nhay nhay
        lcd.print("  ");
        delay(200);
        lcd.setCursor(0, 1);
        lcd.print(Hor);
        if (Hor <=9)
            lcd.print("0");
        lcd.print(":");
        lcd.print("  ");
        //lcd.print(":");
        //lcd.print(Sec);
        //lcd.print("  ");
        delay(200);
        if (digitalRead(set) == 0) {
            mIn = Min;
            EEPROM.write(addr++, mIn);
            tmp = 0;
            while (digitalRead(set) == 0);
        }
    }
    off = 1;
    delay(10);
}

//////////keu chuong//////////
void Buz() {
    if (digitalRead(set) == 0)  //an nut so 3 de tat bao thuc
        off = 0;
    if (off == 1) {
        digitalWrite(buz, HIGH);
        delay(500);

        digitalWrite(buz, LOW);
        delay(500);
    }
}

//////////ham so sanh thoi gian dat bao thuc voi thoi gian thuc//////////
void TimeCheck() {
    int tem[17];
    for (int i = 11; i < 17; i++) {	//doc du lieu da luu vao eeprom
        tem[i] = EEPROM.read(i);
    }
    if (Hor == tem[11] && Min == tem[12] && off == 1) {
        addr = 11;
        Buz();
        Buz();
        lcd.clear();
        lcd.print("alarm...........");
        lcd.setCursor(0, 1);
        lcd.print("...........alarm");
        Buz();
        Buz();
    }
}

//////////bat dau khoi dong//////////
void setup() {
    Wire.begin();
    RTC.begin();
    lcd.begin(16, 2); //so cot va hang LCD
    pinMode(cge, INPUT);
    pinMode(set, INPUT);
    pinMode(mod, INPUT);
    pinMode(buz, OUTPUT);
    digitalWrite(set, HIGH);  //10
    digitalWrite(mod, HIGH);  //9
    digitalWrite(cge, HIGH);  //8

    lcd.setCursor(0, 0);  //(0,0) o 1 dong 1 tren lcd
    lcd.print("PROJECT:");
    lcd.setCursor(0, 1);  //(0,1) o 1 dong 2 tren lcd
    lcd.print("    ALARM_v0");
    delay(1500);

    if (!RTC.isrunning()) { //neu RTC ko chay thi dat lai gio cho no
        RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

//////////hien thi thoi gian thuc ra man hinh//////////
void loop() {
    DateTime now = RTC.now();
    if (digitalRead(mod) == 0) {
        //current();
        addr=11;
        time();
        delay(1000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("   Alarm On!");
        delay(2000);
    }
    lcd.clear();
    
    lcd.setCursor(0, 0);
    lcd.print("Time:");
    lcd.setCursor(6, 0);
    
    Hor = now.hour(), DEC;
    if (Hor <= 9) {
        lcd.print("0");
        lcd.print(Hor = now.hour(), DEC);
    } else
        lcd.print(Hor = now.hour(), DEC);
        
    lcd.print(":");
    Min = now.minute(), DEC;
    if (Min <= 9) {
        lcd.print("0");
        lcd.print(Min = now.minute(), DEC);
    } else
        lcd.print(Min = now.minute(), DEC);

    lcd.print(":");
    Sec = now.second(), DEC;
    if (Sec <= 9) {
        lcd.print("0");
        lcd.print(Sec = now.second(), DEC);
    } else
        lcd.print(Sec = now.second(), DEC);
        
    lcd.setCursor(0, 1);
    lcd.print("Date: ");
    lcd.print(now.day(), DEC);
    lcd.print("/");
    lcd.print(now.month(), DEC);
    lcd.print("/");
    lcd.print(now.year(), DEC);

    //check gio neu dung thi keu
    TimeCheck();
    delay(500); //tan so quet man hinh
}
/*
//////////ham lay thoi gian thuc//////////
void current() {
    lcd.setCursor(0, 1);
    lcd.print(Hor);
    lcd.print(":");
    lcd.print(Min);
    //lcd.print(":");
    //lcd.print(Sec);
}*/
