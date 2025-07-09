#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LED_PIN 8
#define BUZZER_PIN 9

#define BUTTON_P1 7
#define BUTTON_P2 6
#define MODE_BUTTON 5
#define DIFF_BUTTON 4

LiquidCrystal_I2C lcd(0x27, 16, 2);

enum Mode { MODE_2P, MODE_AI };
Mode currentMode = MODE_2P;

enum Difficulty { EASY, MEDIUM, HARD };
Difficulty aiLevel = EASY;

unsigned long p1Times[5] = {0};
unsigned long p2Times[5] = {0};
int p1Index = 0, p2Index = 0;

unsigned long lastModePress = 0;
unsigned long lastDiffPress = 0;

void setup() {
      pinMode(LED_PIN, OUTPUT);
        pinMode(BUZZER_PIN, OUTPUT);
          pinMode(BUTTON_P1, INPUT_PULLUP);
            pinMode(BUTTON_P2, INPUT_PULLUP);
              pinMode(MODE_BUTTON, INPUT_PULLUP);
                pinMode(DIFF_BUTTON, INPUT_PULLUP);

                  lcd.begin(16, 2);
                    lcd.backlight();
                      lcd.setCursor(0, 0);
                        lcd.print("Reaction Tester");
                          delay(2000);
                            lcd.clear();
                            }

                            void loop() {
                                  handleModeButton();
                                    if (currentMode == MODE_AI) {
                                            handleDifficultyButton();
                                    }

                                      lcd.setCursor(0, 0);
                                        lcd.print(currentMode == MODE_2P ? "Mode: 2-Player" : "Mode: AI       ");

                                          lcd.setCursor(0, 1);
                                            if (currentMode == MODE_AI) {
                                                    lcd.print("AI: ");
                                                        if (aiLevel == EASY) lcd.print("Easy   ");
                                                            else if (aiLevel == MEDIUM) lcd.print("Medium ");
                                                                else lcd.print("Hard   ");
                                            } else {
                                                    lcd.print("                ");
                                            }

                                              delay(2000);
                                                lcd.clear();

                                                  lcd.setCursor(0, 0);
                                                    lcd.print("Get Ready...");
                                                      delay(random(3000, 6000));
                                                        lcd.clear();

                                                          digitalWrite(LED_PIN, HIGH);
                                                            tone(BUZZER_PIN, 1000, 100);  //  Start signal
                                                              unsigned long startTime = millis();

                                                                unsigned long aiReactTime = 0;
                                                                  if (currentMode == MODE_AI) {
                                                                        aiReactTime = generateAIReaction();
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

                                                                                        // Winner logic + sound
                                                                                          String winner;
                                                                                            if (p1Pressed && (!p2Pressed || p1Time < p2Time)) {
                                                                                                    winner = "Player 1 Wins!";
                                                                                                        tone(BUZZER_PIN, 800, 300);  //  P1 win tone
                                                                                                            p1Times[p1Index++] = p1Time;
                                                                                                                if (p1Index >= 5) p1Index = 0;
                                                                                            } else {
                                                                                                    winner = (currentMode == MODE_2P) ? "Player 2 Wins!" : "AI Wins!";
                                                                                                        tone(BUZZER_PIN, 600, 300);  //  P2 or AI win tone
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

                                                                                                                                // === Function: Average ===
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

                                                                                                                                        // === Function: Handle Mode Button ===
                                                                                                                                        void handleModeButton() {
                                                                                                                                              if (digitalRead(MODE_BUTTON) == LOW && millis() - lastModePress > 1000) {
                                                                                                                                                    currentMode = (currentMode == MODE_2P) ? MODE_AI : MODE_2P;
                                                                                                                                                        tone(BUZZER_PIN, 1200, 100);  //  Mode switch feedback
                                                                                                                                                            lastModePress = millis();
                                                                                                                                              }
                                                                                                                                            }

                                                                                                                                            // === Function: Handle Difficulty Button ===
                                                                                                                                            void handleDifficultyButton() {
                                                                                                                                                  if (digitalRead(DIFF_BUTTON) == LOW && millis() - lastDiffPress > 1000) {
                                                                                                                                                        aiLevel = (Difficulty)((aiLevel + 1) % 3);
                                                                                                                                                            tone(BUZZER_PIN, 1500, 100);  //  Difficulty switch
                                                                                                                                                                lastDiffPress = millis();
                                                                                                                                                  }
                                                                                                                                                  }

                                                                                                                                                  // === Function: Generate AI Reaction Time ===
                                                                                                                                                  unsigned long generateAIReaction() {
                                                                                                                                                      switch (aiLevel) {
                                                                                                                                                            case EASY: return random(600, 901);
                                                                                                                                                                case MEDIUM: return random(400, 601);
                                                                                                                                                                    case HARD: return random(200, 401);
                                                                                                                                                      }
                                                                                                                                                        return 800;
                                                                                                                                                        }
                                                                                                                                                        