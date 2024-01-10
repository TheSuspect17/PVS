int sensorPin = 4; // Пин, к которому подключен датчик пламени
int flame; // Переменная для хранения значения с датчика
int buzzerPin = 8; // Пин для подключения пищалки
int ledPin = 7; // Пин для подключения красного светодиода

void setup()
{
  pinMode(sensorPin, INPUT); // Устанавливаем пин датчика на вход
  pinMode(buzzerPin, OUTPUT); // Устанавливаем пин пищалки на вывод
  pinMode(ledPin, OUTPUT); // Устанавливаем пин светодиода на вывод
  Serial.begin(9600); // Инициализируем последовательную связь с компьютером на скорости 9600 бит/с
}

void loop()
{
  flame = digitalRead(sensorPin); // Считываем значение с датчика пламени

  if (flame == 1)
  {
    Serial.println("Обнаружено пламя");
    digitalWrite(buzzerPin, LOW); // Выключаем пищалку
    digitalWrite(ledPin, LOW); // Выключаем светодиод
    delay(150); // Пауза 150 миллисекунд
  }
  else
  {
    Serial.println("Пламя не обнаружено");
    
    // Звуковой сигнал пищалки при отсутствии пламени
    digitalWrite(buzzerPin, HIGH); // Включаем пищалку
    delay(100); // Пауза во время звука пищалки
    digitalWrite(buzzerPin, LOW); // Выключаем пищалку

    // Мигание красного светодиода при отсутствии пламени
    for (int i = 0; i < 3; i++) {
      digitalWrite(ledPin, HIGH); // Включаем светодиод
      delay(500); // Пауза во время включенного светодиода
      digitalWrite(ledPin, LOW); // Выключаем светодиод
      delay(500); // Пауза во время выключенного светодиода
    }
  }

  delay(500); // Пауза 500 миллисекунд
}
