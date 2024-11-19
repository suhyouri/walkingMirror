String inputString = ""; // 시리얼 입력을 저장할 변수

//dubugging
const int ledPin1 = 8;    // 첫 번째 LED 핀 번호 (상태 1 확인용)
const int ledPin2 = 9;    // 두 번째 LED 핀 번호 (상태 2 확인용)
const int ledPin3 = 10;    // 세 번째 LED 핀 번호 (상태 3 확인용)


const int frontLeftWheel_dir1_A = 41; // 앞 왼쪽 바퀴 핀 번호(IN1)
const int frontLeftWheel_dir2_A = 43; // 앞 왼쪽 바퀴 핀 번호(IN2)
// const int frontLeftWheel_speed_A = A2; // 모터 속도 제어를 위한 PWM 핀 (ENA)
const int frontLeftWheel_speed_A = 45;

const int frontRightWheel_dir1_B = 40; // 앞 오른쪽 바퀴 핀 번호
const int frontRightWheel_dir2_B = 42; // 앞 오른쪽 바퀴 핀 번호
// const int frontRightWheel_speed_B = A5; // 모터 속도 제어를 위한 PWM 핀 (ENA)
const int frontRightWheel_speed_B = 44;

const int rearLeftWheel_dir1_C = 6; // 뒤 왼쪽 바퀴 핀 번호
const int rearLeftWheel_dir2_C = 7; // 뒤 왼쪽 바퀴 핀 번호
const int rearLeftWheel_speed_C = 5; // 모터 속도 제어를 위한 PWM 핀 (ENA)

const int rearRightWheel_dir1_D = 12; // 뒤 오른쪽 바퀴 핀 번호
const int rearRightWheel_dir2_D = 13; // 뒤 오른쪽 바퀴 핀 번호
const int rearRightWheel_speed_D = 11; // 모터 속도 제어를 위한 PWM 핀 (ENA)

int speed = 240;
int speed2 = 250;
bool isWalkingForward = false; // 전진 상태를 나타내는 변수
bool isWalkingBackward = false; // 전진 상태를 나타내는 변수



void setup()
{
  Serial.begin(9600);  // PC와의 시리얼 통신 시작 (9600bps)
  Serial1.begin(9600); // 블루투스와의 시리얼 통신 시작 (9600bps) 18(초),19번(노)핀임 
  Serial.println("HM-10 블루투스 모듈에 연결 중...");

  pinMode(frontLeftWheel_dir1_A, OUTPUT);
  pinMode(frontLeftWheel_dir2_A, OUTPUT);
  pinMode(frontLeftWheel_speed_A, OUTPUT);

  pinMode(frontRightWheel_dir1_B, OUTPUT);
  pinMode(frontRightWheel_dir2_B, OUTPUT);
  pinMode(frontRightWheel_speed_B, OUTPUT);

  pinMode(rearLeftWheel_dir1_C, OUTPUT);
  pinMode(rearLeftWheel_dir2_C, OUTPUT);
  pinMode(rearLeftWheel_speed_C, OUTPUT);

  pinMode(rearRightWheel_dir1_D, OUTPUT);
  pinMode(rearRightWheel_dir2_D, OUTPUT);
  pinMode(rearRightWheel_speed_D, OUTPUT);

  pinMode(ledPin1, OUTPUT);          // 첫 번째 LED를 출력 모드로 설정
  pinMode(ledPin2, OUTPUT);          // 두 번째 LED를 출력 모드로 설정
  pinMode(ledPin3, OUTPUT);          // 두 번째 LED를 출력 모드로 설정
}

void loop()
{
  // Serial1 핀에 입력이 들어오면, 바이트 단위로 읽어서 PC로 출력
  //
  if (Serial1.available()) {
    char c = Serial1.read();

    if (c != '\n' && c != '\r') {
      inputString += c;
    }
    
    if (c == '\n') {
      Serial.print("Received: ");
      Serial.println(inputString);

      if (inputString.length() == 1) { // 길이가 1일 때만 단일 문자로 변환
        char singleChar = inputString.charAt(0); // 첫 번째 문자 추출

        digitalWrite(ledPin1, LOW);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, LOW);

        switch (singleChar) {
          case 'a': // 정지 명령
            Serial.println("stop");
            stopAllWheels();
            isWalkingForward = false; // 전진 중지
            isWalkingBackward = false;
            digitalWrite(ledPin1, HIGH);
            break;
          case 'b': // 전진 명령
            Serial.println("walk");
            isWalkingForward = true; // 전진 시작
            digitalWrite(ledPin2, HIGH);
            break;
          case 'c':
            Serial.println("back-walk");
            isWalkingBackward = true;
            digitalWrite(ledPin3, HIGH);
            break;
          default:
            Serial.println("nothing set");
            break;
        }
      } else {
        Serial.println("Invalid command");
      }
      
      inputString = ""; // 문자열 초기화
    }
  }

  if (isWalkingForward) {
    walkingForward();
  }

  if (isWalkingBackward) {
    walkingBackward();
  }

  // Serial 핀에 입력이 들어오면, 바이트 단위로 읽어서 블루투스로 출력
  if (Serial.available()) {
    char c = Serial.read();
    Serial1.write(c);
  }
}

void walkingForward(){
  activateFrontWheels();
  delay(400);
  stopAllWheels();
  delay(100);
  activateRearWheels();
  delay(400);
}

void walkingBackward(){
  backRearWheels();
  delay(400);
  stopAllWheels();
  delay(100);
  backFrontWheels();
  delay(400);
}

// // 전륜 먼저 가속 후 후륜 가속
// void walkingForward(int targetSpeed) {
//   activateFrontWheelsGradually(targetSpeed); // 전륜을 천천히 가속
//   delay(300);                                // 전륜이 목표 속도에 도달한 후 잠시 대기
//   stopAllWheels();
//   delay(300); 
//   activateRearWheelsGradually(targetSpeed);  // 후륜을 천천히 가속
//   delay(300);
// }

// 모든 바퀴 정지
void stopAllWheels() {
  // Serial.println("Stop All");
  digitalWrite(frontLeftWheel_dir1_A, LOW);
  digitalWrite(frontLeftWheel_dir2_A, LOW);

  digitalWrite(frontRightWheel_dir1_B, LOW);
  digitalWrite(frontRightWheel_dir2_B, LOW);

  digitalWrite(rearLeftWheel_dir1_C, LOW);
  digitalWrite(rearLeftWheel_dir2_C, LOW);

  digitalWrite(rearRightWheel_dir1_D, LOW);
  digitalWrite(rearRightWheel_dir2_D, LOW);
}

// 전륜(앞 바퀴)만 앞으로 작동
void activateFrontWheels() {
  // Serial.println("Front Wheel");
  digitalWrite(frontLeftWheel_dir1_A, HIGH);
  digitalWrite(frontLeftWheel_dir2_A, LOW);
  analogWrite(frontLeftWheel_speed_A, speed);  

  digitalWrite(frontRightWheel_dir1_B, HIGH);
  digitalWrite(frontRightWheel_dir2_B, LOW);
  analogWrite(frontRightWheel_speed_B, speed);

  digitalWrite(rearLeftWheel_dir1_C, LOW);
  digitalWrite(rearLeftWheel_dir2_C, LOW);

  digitalWrite(rearRightWheel_dir1_D, LOW);
  digitalWrite(rearRightWheel_dir2_D, LOW);
}

// 후륜(뒤 바퀴)만 앞으로 작동
void activateRearWheels() {
  // Serial.println("Rear Wheel");
  digitalWrite(frontLeftWheel_dir1_A, LOW);
  digitalWrite(frontLeftWheel_dir2_A, LOW);

  digitalWrite(frontRightWheel_dir1_B, LOW);
  digitalWrite(frontRightWheel_dir2_B, LOW);

  digitalWrite(rearLeftWheel_dir1_C, HIGH);
  digitalWrite(rearLeftWheel_dir2_C, LOW);
  analogWrite(rearLeftWheel_speed_C, speed);   

  digitalWrite(rearRightWheel_dir1_D, HIGH);
  digitalWrite(rearRightWheel_dir2_D, LOW);
  analogWrite(rearRightWheel_speed_D, speed);   
}

// 전륜(앞 바퀴)만 뒤로 작동
void backFrontWheels() {
  // Serial.println("Front Wheel");
  digitalWrite(frontLeftWheel_dir1_A, LOW);
  digitalWrite(frontLeftWheel_dir2_A, HIGH);
  analogWrite(frontLeftWheel_speed_A, speed);   // 속도 200

  digitalWrite(frontRightWheel_dir1_B, LOW);
  digitalWrite(frontRightWheel_dir2_B, HIGH);
  analogWrite(frontRightWheel_speed_B, speed);   // 속도 200

  digitalWrite(rearLeftWheel_dir1_C, LOW);
  digitalWrite(rearLeftWheel_dir2_C, LOW);

  digitalWrite(rearRightWheel_dir1_D, LOW);
  digitalWrite(rearRightWheel_dir2_D, LOW);
}

// 후륜(뒤 바퀴)만 뒤로 작동
void backRearWheels() {
  // Serial.println("Rear Wheel");
  digitalWrite(frontLeftWheel_dir1_A, LOW);
  digitalWrite(frontLeftWheel_dir2_A, LOW);

  digitalWrite(frontRightWheel_dir1_B, LOW);
  digitalWrite(frontRightWheel_dir2_B, LOW);

  digitalWrite(rearLeftWheel_dir1_C, LOW);
  digitalWrite(rearLeftWheel_dir2_C, HIGH);
  analogWrite(rearLeftWheel_speed_C, speed);   // 속도 200

  digitalWrite(rearRightWheel_dir1_D, LOW);
  digitalWrite(rearRightWheel_dir2_D, HIGH);
  analogWrite(rearRightWheel_speed_D, speed);   // 속도 200
}


// 전륜(앞 바퀴)만 앞으로 천천히 작동
void activateFrontWheelsGradually(int targetSpeed) {
  int currentSpeed = 100; // 초기 속도를 0으로 설정
  digitalWrite(rearLeftWheel_dir1_C, LOW);
  digitalWrite(rearLeftWheel_dir2_C, LOW);

  digitalWrite(rearRightWheel_dir1_D, LOW);
  digitalWrite(rearRightWheel_dir2_D, LOW);

  // 속도를 0에서 targetSpeed까지 점진적으로 증가
  while (currentSpeed <= targetSpeed) {
    // 왼쪽 앞 바퀴 설정
    digitalWrite(frontLeftWheel_dir1_A, HIGH);
    digitalWrite(frontLeftWheel_dir2_A, LOW);
    analogWrite(frontLeftWheel_speed_A, currentSpeed);

    // 오른쪽 앞 바퀴 설정
    digitalWrite(frontRightWheel_dir1_B, HIGH);
    digitalWrite(frontRightWheel_dir2_B, LOW);
    analogWrite(frontRightWheel_speed_B, currentSpeed);
    
    currentSpeed += 30;  // 속도를 10씩 증가 (필요에 따라 조정 가능)
    delay(50);           // 각 단계 간 지연 시간 (밀리초 단위)
  }
}

// 후륜(뒤 바퀴)만 앞으로 천천히 작동
void activateRearWheelsGradually(int targetSpeed) {
  int currentSpeed = 100; // 초기 속도를 0으로 설정
  digitalWrite(frontLeftWheel_dir1_A, LOW);
  digitalWrite(frontLeftWheel_dir2_A, LOW);

  digitalWrite(frontRightWheel_dir1_B, LOW);
  digitalWrite(frontRightWheel_dir2_B, LOW);

  // 속도를 0에서 targetSpeed까지 점진적으로 증가
  while (currentSpeed <= targetSpeed) {
    // 왼쪽 뒤 바퀴 설정
    digitalWrite(rearLeftWheel_dir1_C, HIGH);
    digitalWrite(rearLeftWheel_dir2_C, LOW);
    analogWrite(rearLeftWheel_speed_C, currentSpeed);


    // 오른쪽 뒤 바퀴 설정
    digitalWrite(rearRightWheel_dir1_D, HIGH);
    digitalWrite(rearRightWheel_dir2_D, LOW);
    analogWrite(rearRightWheel_speed_D, currentSpeed);

    currentSpeed += 30;  // 속도를 10씩 증가 (필요에 따라 조정 가능)
    delay(50);           // 각 단계 간 지연 시간 (밀리초 단위)
  }
}