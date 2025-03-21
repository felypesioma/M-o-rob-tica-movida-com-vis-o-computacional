#include <Servo.h> // Inclui a biblioteca Servo.h

char cmd;        // Variável para armazenar o comando recebido via Serial
char valor;      // Variável para armazenar o valor do comando recebido
Servo indicador; // Objeto servo para o indicador
Servo meio;      // Objeto servo para o meio
Servo dedao;     // Objeto servo para o polegar
Servo anelar;    // Objeto servo para o anelar
Servo mínimo;  // Objeto servo para o mínimo

// Define os ângulos finais personalizados para cada servo
const int anguloFinalIndicador = 20;
const int anguloFinalMeio = 45;
const int anguloFinalDedao = 45;
const int anguloFinalAnelar = 70;
const int anguloFinalMínimo = 70;

void setup() {
  Serial.begin(9600);       // Inicializa a comunicação serial

  // Anexa os servos aos pinos correspondentes
  indicador.attach(11);
  
  meio.attach(10);
  dedao.attach(9);
  anelar.attach(6);
  mínimo.attach(5);

  // Define posições iniciais (0 graus) para os servos
  indicador.write(0);
  meio.write(0);
  dedao.write(0);
  anelar.write(0);
  mínimo.write(0);
}

void loop() {
  if (Serial.available() >= 2) { // Verifica se há pelo menos 2 bytes disponíveis (comando + valor)
    cmd = Serial.read();         // Lê o comando recebido ('i', 'm', 'd', 'a', 'n')
    valor = Serial.read();       // Lê o próximo caractere ('0' ou '1')

    // Determina qual servo mover com base no comando recebido
    switch (cmd) {
      case 'i': // Comando para o servo indicador
        moverServo(indicador, valor, "Indicador", anguloFinalIndicador);
        break;
      case 'm': // Comando para o servo meio
        moverServo(meio, valor, "Meio", anguloFinalMeio);
        break;
      case 'd': // Comando para o servo polegar
        moverServo(dedao, valor, "polegar", anguloFinalDedao);
        break;
      case 'a': // Comando para o servo anelar
        moverServo(anelar, valor, "Anelar", anguloFinalAnelar);
        break;
      case 'n': // Comando para o servo mínimo
        moverServo(mínimo, valor, "Mínimo", anguloFinalMínimo);
        break;
      default:
        Serial.println("Comando inválido.");
        break;
    }
  }
}

// Função para mover o servo e imprimir mensagens
void moverServo(Servo &servo, char valor, const char *nome, int anguloFinal) {
  if (valor == '1') {
    servo.write(anguloFinal); // Move o servo para o ângulo final
    Serial.print(nome);
    Serial.print(": ");
    Serial.print(anguloFinal);
    Serial.println(" graus");
  } else if (valor == '0') {
    servo.write(0);  // Move o servo para 0 graus
    Serial.print(nome);
    Serial.println(": 0 graus");
  } else {
    Serial.print("Comando inválido para ");
    Serial.println(nome);
  }
}

