#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHTesp.h>
#include <WiFi.h>
#include <time.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGTH 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

#define BUZZER 18
#define LED_1 15
#define LED_2 2
#define CANCEL 34
#define UP 35
#define DOWN 32
#define OK 33
#define DHT 12

#define NTP_SERVER    "pool.ntp.org"
#define UTC_OFFSET_DST 0
int UTC_OFFSET = 0;

//Declare the objects
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGTH, &Wire, OLED_RESET);
DHTesp dhtSensor;

//Global time variables
int days = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;

unsigned long timeNow = 0;
unsigned long timeLast = 0;

bool alarm_enabled = false;
int n_alarms = 2;
int alarm_hours[] = {0, 0};
int alarm_minutes[] = {0, 0};
bool alarm_triggered[] = {false, false};

// variables of notes 
int n_notes = 8;
int C = 262;
int D = 294;
int E = 330;
int F = 349;
int G = 392;
int A = 440;
int B = 494;
int C_H = 523;
int notes[] = {C, D, E, F, G, A, B, C_H};

int current_mode = 0;
int max_modes = 5;
String modes[] = {"1 - Set Time Zone", "2 - Set Alarm 1", "3 - Set Alarm 2", "5 - Set Alarm 3", "4 - Remove Alarm"};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(BUZZER, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(CANCEL, INPUT);
  pinMode(UP, INPUT);
  pinMode(DOWN, INPUT);
  pinMode(OK, INPUT);

  dhtSensor.setup(DHT, DHTesp::DHT22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println(F("SSD1306 allocaiton failed"));
    for (;;);
  }
  display.display();
  delay(2000);

  display.clearDisplay();
  print_line("Welcome to Medibox", 2, 0, 0);
  delay(3000);

  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    display.clearDisplay();
    print_line("Connecting to WiFi", 2, 0, 0);
  }

  display.clearDisplay();
  print_line("Connected to WiFi", 2, 0, 0);

}

void loop() {

  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);

  update_time_with_check_alarm();
  if (digitalRead(CANCEL) == LOW) {
    delay(1000);
    Serial.println("Menu");
    go_to_menu();
  }

  check_temp();
}

//defining the functions
void print_line(String text, int text_size, int row, int column) {
  display.setTextSize(text_size);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(column, row);
  display.println(text);

  display.display();
}

void update_time(void) {
  struct tm timeinfo;
  getLocalTime(&timeinfo);

  char day_str[8];
  char hour_str[8];
  char min_str[8];
  char sec_str[8];
  strftime(day_str, 8, "%d", &timeinfo);
  strftime(sec_str, 8, "%S", &timeinfo);
  strftime(hour_str, 8,"%H", &timeinfo);
  strftime(min_str, 8, "%M", &timeinfo);

  hours = atoi(hour_str);
  minutes = atoi(min_str);
  days = atoi(day_str);
  seconds = atoi(sec_str);
}


void ring_alarm() {
  display.clearDisplay();
  print_line("MEDICINE TIME!", 0, 0, 2);


  digitalWrite(LED_1, HIGH);
  bool break_happpened = false;
  // Ring the buzzer
  while (break_happpened == false && digitalRead(CANCEL) == HIGH) {


    for (int i = 0 ; i < n_notes; i++) {
      tone(BUZZER, notes[i]);
      delay(500);
      noTone(BUZZER);
      delay(2);
      if (digitalRead(CANCEL) == LOW) {
        delay (200);
        break_happpened = true;
        break;
      }
    }
  }
}

void update_time_with_check_alarm(void) {
  display.clearDisplay();
  update_time();
  print_time_now();

  if (alarm_enabled) {
    for (int i = 0; i < n_alarms; i++) {
      if (alarm_triggered[i] == false && alarm_hours[i] && minutes == alarm_minutes[i]) {
        ring_alarm();
        alarm_triggered[i] = true;
      }
    }
  }
}

int wait_for_button_press() {
  while (true) {
    if (digitalRead(UP) == LOW) {
      delay(200);
      return UP;
    }

    else if (digitalRead(DOWN) == LOW) {
      delay(200);
      return DOWN;
    }

    else if (digitalRead(CANCEL) == LOW) {
      delay(200);
      return CANCEL;
    }

    else if (digitalRead(OK) == LOW) {
      delay(200);
      return OK;
    }

    update_time();
  }
}

void go_to_menu() {
  while (digitalRead(CANCEL) == HIGH) {
    display.clearDisplay();
    print_line(modes[current_mode], 0, 0, 2);

    int pressed = wait_for_button_press();
    if (pressed == UP) {
      current_mode += 1;
      current_mode %= max_modes;
      delay(200);
    }

    else if (pressed == DOWN) {
      current_mode -= 1;
      if (current_mode < 0) {
        current_mode = max_modes - 1;
      }
      delay(200);
    }

    else if (pressed == OK) {
      Serial.println(current_mode);
      delay(200);
      run_mode(current_mode);
    }

    else if (pressed == CANCEL) {
      delay(200);
      break;
    }

  }
}

void run_mode(int mode){
  if (mode == 0) {
    set_time_zone();
  }

  else if (mode == 1 || mode == 2 || mode == 3) {
    set_alarm(mode - 1);
  }

  else if (mode == 4) {
    alarm_enabled = false;
  }
}

void set_time_zone() {

  int temp_hour = 0;
  int cnvt_hour = 0;
  while (true) {
    display.clearDisplay();
    print_line("Enter UTC hour: " + String(temp_hour), 0, 0, 2);

    int pressed = wait_for_button_press();
    if (pressed == UP) {
      delay(200);
      temp_hour += 1;
      if (temp_hour > 14) {
        temp_hour = -12;
      }
    }

    else if (pressed == DOWN) {
      delay(200);
      temp_hour -= 1;
      if (temp_hour < -12) {
        temp_hour = 14;
      }
    }

    else if (pressed == OK) {
      delay(200);
      cnvt_hour = temp_hour;
      break;
    }

    else if (pressed == CANCEL) {
      delay(200);
      break;
    }
  }

  int temp_minute = 0;
  int cnvt_minute = 0;
  while (true) {
    display.clearDisplay();
    print_line("Enter UTC minutes: " + String(temp_minute), 0, 0, 2);

    int pressed = wait_for_button_press();
    if (pressed == UP) {
      delay(200);
      temp_minute += 1;
      temp_minute = temp_minute % 60;
    }

    else if (pressed == DOWN) {
      delay(200);
      temp_minute -= 1;
      if (temp_minute < 0) {
        temp_minute = 59;
      }
    }

    else if (pressed == OK) {
      delay(200);
      cnvt_minute = temp_minute;
      break;
    }

    else if (pressed == CANCEL) {
      delay(200);
      break;
    }
  }

  UTC_OFFSET = ((cnvt_hour * 3600) + (cnvt_minute * 60));

  display.clearDisplay();
  print_line("Time is set", 0, 0, 2);
  delay(1000);
}

void set_alarm(int alarm) {

  int temp_hour = alarm_hours[alarm];
  while (true) {
    display.clearDisplay();
    print_line("Enter hour: " + String(temp_hour), 0, 0, 2);

    int pressed = wait_for_button_press();
    if (pressed == UP) {
      delay(200);
      temp_hour += 1;
      temp_hour = temp_hour % 24;
    }

    else if (pressed == DOWN) {
      delay(200);
      temp_hour -= 1;
      if (temp_hour < 0) {
        temp_hour = 23;
      }
    }

    else if (pressed == OK) {
      delay(200);
      alarm_hours[alarm] = temp_hour;
      break;
    }

    else if (pressed == CANCEL) {
      delay(200);
      break;
    }
  }

  int temp_minute = alarm_minutes[alarm];
  while (true) {
    display.clearDisplay();
    print_line("Enter minute: " + String(temp_minute), 0, 0, 2);

    int pressed = wait_for_button_press();
    if (pressed == UP) {
      delay(200);
      temp_minute += 1;
      temp_minute = temp_minute % 60;
    }

    else if (pressed == DOWN) {
      delay(200);
      temp_minute -= 1;
      if (temp_minute < 0) {
        temp_minute = 59;
      }
    }

    else if (pressed == OK) {
      delay(200);
      alarm_minutes[alarm] = temp_minute;
      break;
    }

    else if (pressed == CANCEL) {
      delay(200);
      break;
    }
  }

  display.clearDisplay();
  print_line("Alarm is set", 0, 0, 2);
  delay(1000);
}

void check_temp(void) {
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  bool all_good = true;

  print_line(String(data.temperature), 90, 40, 0);
  print_line(String(data.humidity), 90, 50, 0);

  if (data.temperature > 35) {
    all_good = false;
    digitalWrite(LED_2, HIGH);
    print_line("TEMP HIGH", 1, 40, 0);
  }

  else if (data.temperature < 25) {
    all_good = false;
    digitalWrite(LED_2, HIGH);
    print_line("TEMP LOW", 1, 40, 0);
  }

  if (data.humidity > 85) {
    all_good = false;
    digitalWrite(LED_2, HIGH);
    print_line("HUMD HIGH", 1, 50, 0);
  }

  else if (data.humidity < 35) {
    all_good = false;
    digitalWrite(LED_2, HIGH);
    print_line("HUMD LOW", 1, 50, 0);
  }

  if (all_good) {
    digitalWrite(LED_2, LOW);
  }
}

void print_time_now(void) {
  print_line(String(days), 2, 0, 0);
  print_line("-", 2, 0, 20);
  print_line(String(hours), 2, 0, 30);
  print_line(":", 2, 0, 50);
  print_line(String(minutes), 2, 0, 60);
  print_line(":", 2, 0, 80);
  print_line(String(seconds), 2, 0, 90);
}
