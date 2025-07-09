#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// === Pin Setup ===
#define LED_PIN 8
#define BUZZER_PIN 9
#define BUTTON_P1 7
#define BUTTON_P2 6
#define MODE_BUTTON 5
#define DIFF_BUTTON 4
#define SELECT_BUTTON 3

LiquidCrystal_I2C lcd(0x27, 16, 2);

// === Game State ===
enum Mode { MODE_2P, MODE_AI };
enum Difficulty { EASY, MEDIUM, HARD };

Mode currentMode = MODE_2P;
Difficulty aiLevel = EASY;

unsigned long p1Times[5] = {0};
unsigned long p2Times[5] = {0};
int p1Index = 0, p2Index = 0;

int selectedMode = 0;          // 0 = 2P, 1 = AI
int selectedDifficulty = 0;    // 0 = Easy, 1 = Med, 2 = Hard
bool showMenuAgain = true;

// === Function Prototypes ===
void displayModeSelection();
void displayDifficultySelection();
void playGame();
void showCountdown();
float average(unsigned long arr[]);

void setup() {
    pinMode(LED_PIN, OUTPUT);
      pinMode(BUZZER_PIN, OUTPUT);
        pinMode(BUTTON_P1, INPUT_PULLUP);
          pinMode(BUTTON_P2, INPUT_PULLUP);
            pinMode(MODE_BUTTON, INPUT_PULLUP);
              pinMode(DIFF_BUTTON, INPUT_PULLUP);
                pinMode(SELECT_BUTTON, INPUT_PULLUP);

                  lcd.init();
                    lcd.backlight();
                      lcd.setCursor(0, 0);
                        lcd.print("Reaction Tester");
                          delay(2000);
                            lcd.clear();
}

void loop() {
    if (showMenuAgain) {
          displayModeSelection();
              if (currentMode == MODE_AI) {
                      displayDifficultySelection();
              }
                  showMenuAgain = false;
            }

              playGame();
                showMenuAgain = true;
                  delay(1000);
                  }

                  // === MODE SELECTI SCREEN ===
                  void displayModeSelection() {
                      lcd.clear();
                        lcd.setCursor(0, 0);
                          lcd.print("Select Mode:");

                            int cursor = selectedMode;

                              while (true) {
                                    lcd.setCursor(0, 1);
                                        lcd.print(cursor == 0 ? ">2P  1P     " : " 2P >1P     ");

                                            if (digitalRead(MODE_BUTTON) == LOW) {
                                                    cursor = (cursor + 1) % 2;
                                                          tone(BUZZER_PIN, 1500, 100);
                                                                delay(300);
                                                                      while (digitalRead(MODE_BUTTON) == LOW);
                                            }

                                                if (digitalRead(SELECT_BUTTON) == LOW) {
                                                        selectedMode = cursor;
                                                              currentMode = (cursor == 0) ? MODE_2P : MODE_AI;
                                                                    tone(BUZZER_PIN, 1200, 100);
                                                                          delay(300);
                                                                                while (digitalRead(SELECT_BUTTON) == LOW);
                                                                                      break;
                                                }
                                              }

                                                lcd.clear();
                                            }

                                            // === DIFFICULTSELECTION SCREEN ===
                                            void displayDifficultySelection() {
                                                lcd.clear();
                                                  lcd.setCursor(0, 0);
                                                    lcd.print("Select Difficulty");

                                                      int cursor = selectedDifficulty;
                                                        int cursorPos[3] = {0, 6, 14}; // Easy, Medium, Hard

                                                          while (true) {
                                                                lcd.setCursor(0, 1);
                                                                    lcd.print("Easy  Medium Hard");
                                                                        lcd.setCursor(cursorPos[cursor], 1);
                                                                            lcd.write('>');

                                                                                if (digitalRead(DIFF_BUTTON) == LOW) {
                                                                                        cursor = (cursor + 1) % 3;
                                                                                              tone(BUZZER_PIN, 1500, 100);
                                                                                                    delay(300);
                                                                                                          while (digitalRead(DIFF_BUTTON) == LOW);
                                                                                }

                                                                                    if (digitalRead(SELECT_BUTTON) == LOW) {
                                                                                            selectedDifficulty = cursor;
                                                                                                  aiLevel = (Difficulty)cursor;
                                                                                                        tone(BUZZER_PIN, 1200, 100);
                                                                                                              delay(300);
                                                                                                                    while (digitalRead(SELECT_BUTTON) == LOW);
                                                                                                                          break;
                                                                                    }
                                                                                  }

                                                                                    lcd.clear();
                                                                                    }

                                                                                    // === GAME COUNTDOWN ===
                                                                                    void showCountdown() {
                                                                                        for (int i = 3; i >= 1; i--) {
                                                                                              lcd.clear();
                                                                                                  lcd.setCursor(7, 0);
                                                                                                      lcd.print(i);
                                                                                                          tone(BUZZER_PIN, 1000, 200);
                                                                                                              digitalWrite(LED_PIN, HIGH);
                                                                                                                  delay(200);
                                                                                                                      digitalWrite(LED_PIN, LOW);
                                                                                                                          delay(600);
                                                                                        }

                                                                                          lcd.clear();
                                                                                            lcd.setCursor(5, 0);
                                                                                              lcd.print("GO!");
                                                                                                tone(BUZZER_PIN, 1500, 300);
                                                                                                  digitalWrite(LED_PIN, HIGH);
                                                                                                    delay(500);
                                                                                                      digitalWrite(LED_PIN, LOW);
                                                                                                        lcd.clear();
                                                                                                        }

                                                                                                        // === GAME LOGIC ===
                                                                                                        void playGame() {
                                                                                                            showCountdown();
                                                                                                              delay(random(1000, 3000));

                                                                                                                digitalWrite(LED_PIN, HIGH);
                                                                                                                  tone(BUZZER_PIN, 1000, 100);
                                                                                                                    unsigned long startTime = millis();

                                                                                                                      unsigned long aiReactTime = 0;
                                                                                                                        if (currentMode == MODE_AI) {
                                                                                                                              switch (aiLevel) {
                                                                                                                                      case EASY: aiReactTime = random(600, 901); break;
                                                                                                                                            case MEDIUM: aiReactTime = random(400, 601); break;
                                                                                                                                                  case HARD: aiReactTime = random(200, 401); break;
                                                                                                                              }
                                                                                                                            }

                                                                                                                              bool p1Pressed = false, p2Pressed = false;
                                                                                                                                unsigned long p1Time = 0, p2Time = 0;

                                                                                                                                  while (!p1Pressed && !p2Pressed) {
                                                                                                                                        if (digitalRead(BUTTON_P1) == LOW) {
                                                                                                                                                p1Time = millis() - startTime;
                                                                                                                                                      p1Pressed = true;
                                                                                                                                        }

                                                                                                                                            if (currentMode == MODE_2P && digitalRead(BUTTON_P2) == LOW) {
                                                                                                                                                    p2Time = millis() - startTime;
                                                                                                                                                          p2Pressed = true;
                                                                                                                                            }

                                                                                                                                                if (currentMode == MODE_AI && millis() - startTime >= aiReactTime) {
                                                                                                                                                        p2Time = aiReactTime;
                                                                                                                                                              p2Pressed = true;
                                                                                                                                                }
                                                                                                                                              }

                                                                                                                                                digitalWrite(LED_PIN, LOW);

                                                                                                                                                  String winner;
                                                                                                                                                    if (p1Pressed && (!p2Pressed || p1Time < p2Time)) {
                                                                                                                                                          winner = "Player 1 Wins!";
                                                                                                                                                              tone(BUZZER_PIN, 800, 300);
                                                                                                                                                                  p1Times[p1Index++] = p1Time;
                                                                                                                                                                      if (p1Index >= 5) p1Index = 0;
                                                                                                                                                    } else {
                                                                                                                                                          winner = (currentMode == MODE_2P) ? "Player 2 Wins!" : "AI Wins!";
                                                                                                                                                              tone(BUZZER_PIN, 600, 300);
                                                                                                                                                                  p2Times[p2Index++] = p2Time;
                                                                                                                                                       if (p2Index >= 5) p2Index = 0;
                                                                                                                                                    }

                                                                                                                                                      lcd.setCursor(0, 0);
                                                                                                                                                        lcd.print(winner);
                                                                                                                                                          lcd.setCursor(0, 1);
                                                                                                                                                            lcd.print("P1:");
                                                                                                                                                              lcd.print(p1Time);
                                                                                                                                                                lcd.print(" P2:");
                                                                                                                                                                  lcd.print(p2Time);
                                                                                                                                                                    delay(3000);

                                                                                                                                                                      // Trash Talk
                                                                                                                                                                        lcd.clear();
                                                                                                                                                                          if (p1Time > 800) {
                                                                                                                                                                                lcd.setCursor(0, 0);
                                                                                                                                                                                    lcd.print("P1: TOO SLOWWW!");
                                                                                                                                                                                        lcd.setCursor(0, 1);
                                                                                                                                                                                            lcd.print("Blink much? ");
                                                                                                                                                                                                delay(2000);
                                                                                                                                                                          }

                                                                                                                                                                            if (p2Time > 800) {
                                                                                                                                                                                  lcd.clear();
                                                                                                                                                                                      lcd.setCursor(0, 0);
                                                                                                                                                                                          lcd.print("P2: GRANNY MODE!");
                                                                                                                                                                                              lcd.setCursor(0, 1);
                                                                                                                                                                                                  lcd.print("Zzz... WAKE UP!");
                                                                                                                                                                                                      delay(2000);
                                                                                                                                                                            }

                                                                                                                                                                              // Show averages
                                                                                                                                                                                lcd.clear();
                                                                                                                                                                                  lcd.setCursor(0, 0);
                                                                                                                                                                                    lcd.print("P1 Avg:");
                                                                                                                                                                                      lcd.print(average(p1Times));
                                                                                                                                                                                        lcd.setCursor(0, 1);
                                                                                                                                                                                          lcd.print("P2 Avg:");
                                                                                                                                                                                            lcd.print(average(p2Times));
                                                                                                                                                                                              delay(3000);
                                                                                                                                                                                                lcd.clear();
                                                                                                                                                                                                }

                                                                                                                                                                                                // === AVERAGE CALC ===
                                                                                                                                                                                                float average(unsigned long arr[]) {
                                                                                                                                                                                                    unsigned long sum = 0;
                                                                                                                                                                                                      int count = 0;
                                                                                                                                                                                                        for (int i = 0; i < 5; i++) {
                                                                                                                                                                                                              if (arr[i] != 0) {
                                                                                                                                                                                                                      sum += arr[i];
                                                                                                                                                                                                                            count++;
                                                                                                                                                                                                              }
                                                                                                                                                                                                            }
                                                                                                                                                                                                              return count > 0 ? (float)sum / count : 0;
                                                                                                                                                                                                              }
                                                                                                                                                                                                              