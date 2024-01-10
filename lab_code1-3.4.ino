#define PUMP_PIN 3 // Определение пина, к которому подключен насос
int soilMoistureValue = 0; // Переменная для хранения значения влажности почвы
int percentage = 0; // Переменная для хранения процента влажности

void setup() {
  pinMode(PUMP_PIN, OUTPUT); // Установка пина насоса на вывод
  Serial.begin(9600); // Инициализация последовательной связи с компьютером на скорости 9600 бит/с
}

void loop()
{ 
  soilMoistureValue = analogRead(A0); // Считываем значение влажности почвы с аналогового пина A0
  Serial.println(percentage); // Выводим значение процента влажности на монитор последовательного порта

  // Преобразование значения влажности из диапазона 490-1023 в диапазон 100-0
  percentage = map(soilMoistureValue, 490, 1023, 100, 0);

  if (percentage > 10)
  {
    Serial.println("pump on"); // Включаем насос
    digitalWrite(PUMP_PIN, LOW);
  }

  if (percentage < 80)
  {
    Serial.println("pump off"); // Выключаем насос
    digitalWrite(PUMP_PIN, HIGH);
  }
}