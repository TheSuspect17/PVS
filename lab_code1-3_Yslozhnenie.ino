#include <Wire.h> // Подключение библиотеки Wire для работы с I2C
#include <LiquidCrystal_I2C.h> // Подключение библиотеки LiquidCrystal_I2C для работы с LCD по I2C
#include "DHT.h" // Подключение библиотеки DHT для работы с датчиком DHT
#include "MQ135.h" // Подключение библиотеки MQ135 для работы с датчиком газа

byte degree[8] = { // Описание массива байт для создания специального символа
  B00110, // Значение байта, представляющего строку символа
  B01001, // Продолжаем описание каждой строки символа
  B01001,
  B00110,
  B00000,
  B00000,
  B00000,
  B00000
};

LiquidCrystal_I2C lcd(0x27, 16, 2); // Инициализация объекта для работы с LCD, указываем адрес и размер

#define DHTPIN A1 // Задаем пин, к которому подключен сигнальный вывод датчика DHT
#define DHTTYPE DHT11 // Указываем тип используемого датчика DHT (DHT11)
DHT dht(DHTPIN, DHTTYPE); // Инициализация объекта для работы с датчиком DHT

#define ANALOGPIN A3 // Задаем пин, к которому подключен аналоговый сигнальный пин
MQ135 gasSensor = MQ135(ANALOGPIN); // Инициализация объекта для работы с датчиком газа MQ135

int flameSensorPin = 5; // Задаем пин, к которому подключен датчик пламени

#define PUMP_PIN 3 // Задаем пин, к которому подключен насос

void setup() {
  lcd.init(); // Инициализация LCD
  lcd.backlight(); // Включение подсветки LCD
  lcd.createChar(1, degree); // Создание специального символа на LCD
  Serial.begin(9600); // Инициализация последовательной связи с компьютером на скорости 9600 бит/с
  dht.begin(); // Инициализация датчика DHT
  pinMode(flameSensorPin, INPUT); // Настройка пина датчика пламени на вход
  pinMode(PUMP_PIN, OUTPUT); // Настройка пина насоса на вывод
}

void loop() {
  float h = dht.readHumidity(); // Считывание значения влажности воздуха с датчика DHT
  float t = dht.readTemperature(); // Считывание значения температуры воздуха с датчика DHT

  float ppm = gasSensor.getPPM(); // Получение значения концентрации газа (CO2) с датчика MQ135

  int flame = digitalRead(flameSensorPin); // Считывание значения датчика пламени

  int soilMoistureValue = analogRead(A0); // Считывание значения влажности почвы с аналогового пина A0
  int percentage = map(soilMoistureValue, 490, 1023, 100, 0); // Преобразование значения влажности почвы к процентам

  lcd.setCursor(0, 0); // Установка курсора на LCD в начало первой строки
  lcd.print("Hum = % "); // Вывод текста "Hum = % "
  lcd.setCursor(7, 0); // Установка курсора на LCD на 7-й символ первой строки
  lcd.print(h, 1); // Вывод значения влажности (h) на LCD с 1 знаком после запятой
  lcd.setCursor(0, 1); // Установка курсора на LCD в начало второй строки
  lcd.print("Temp = 1C "); // Вывод текста "Temp = 1C" (1 - специальный символ градуса)
  lcd.setCursor(7, 1); // Установка курсора на LCD на 7-й символ второй строки
  lcd.print(t, 1); // Вывод значения температуры (t) на LCD с 1 знаком после запятой

  delay(2000); // Задержка 2 секунды

  lcd.clear(); // Очистка LCD

  // Вывод информации о датчике пламени
  lcd.setCursor(0, 2); // Установка курсора на LCD в начало третьей строки
  lcd.print("Flame: "); // Вывод текста "Flame: "
  lcd.print(flame ? "Detected " : "Not Detected"); // Вывод значения датчика пламени

  delay(2000); // Задержка 2 секунды

  lcd.clear(); // Очистка LCD

  // Вывод информации о датчике MQ135 (CO2)
  lcd.setCursor(0, 0); // Установка курсора на LCD в начало первой строки
  lcd.print("CO2 ppm: "); // Вывод текста "CO2 ppm: "
  lcd.print(ppm); // Вывод значения концентрации CO2

  delay(2000); // Задержка 2 секунды

  lcd.clear(); // Очистка LCD

  // Вывод информации о последнем датчике (влажности воздуха)
  lcd.setCursor(0, 0); // Установка курсора на LCD в начало первой строки
  lcd.print("Humidity: "); // Вывод текста "Humidity: "
  lcd.print(h); // Вывод значения влажности

  delay(2000); // Задержка 2 секунды

  lcd.clear(); // Очистка LCD 

  Serial.print("Humidity: "); // Вывод информации о влажности на монитор последовательного порта
  Serial.print(h); // Вывод значения влажности
  Serial.print(" %\t"); // Вывод символа "%", табуляции и пробела
  Serial.print("Temperature: "); // Вывод информации о температуре
  Serial.print(t); // Вывод значения температуры
  Serial.println(" C"); // Вывод символа "C" и перевода строки
  Serial.print("CO2 ppm value: "); // Вывод информации о концентрации CO2
  Serial.println(ppm); // Вывод значения концентрации CO2
  Serial.print("Flame detected: "); // Вывод информации о наличии пламени
  Serial.println(flame ? "Yes" : "No"); // Вывод значения датчика пламени
  Serial.print("Soil moisture: "); // Вывод информации о влажности почвы
  Serial.print(percentage); // Вывод значения влажности почвы в процентах
  Serial.println("%"); // Вывод символа "%"
  Serial.print("Humidity value: "); // Вывод информации о значении влажности
  Serial.print(h); // Вывод значения влажности
  Serial.println(); // Перевод строки

  if (percentage > 10) {
    Serial.println("Turning pump on"); // Включение насоса
    digitalWrite(PUMP_PIN, LOW);
  }
  if (percentage < 80) {
    Serial.println("Turning pump off"); // Выключение насоса
    digitalWrite(PUMP_PIN, HIGH);
  }

  delay(2000); // Задержка 2 секунды
}

