// main.cpp 3 rideaux
#include <Arduino.h>
#include <ezButton.h>
#include <AccelStepper.h>




//definition des pin pour les moteurs
#define ENABLE_PIN 8 /* Enable pin for all stepper outputs */ 
#define X_DIR 5 /* Direction-Pin for X-axis */ 
#define X_STEP 2 /* Step-Pin for X-axis */ 
#define X_ENDSTOP_PIN 9 /* Limit switch for X-axis */


// initialisation des positions des moteurs
int X_position = 0;


// definition des paramètres de la LED
const int ledPin = LED_BUILTIN;
bool ledState = LOW;

//Création de l'objet accelStepper
AccelStepper stepperX(AccelStepper::DRIVER, X_STEP, X_DIR);


// pin fin de course
ezButton finDeCourseX(X_ENDSTOP_PIN);


// déclaration des fonctions
void BougerMoteur(AccelStepper* stepper, ezButton* finDeCourse, int MAX, int distance);


void setup() {
  Serial.begin(115200);
  delay(2000); //attente que le moniteur série soit prêt


  // configuration des broches
  pinMode(ledPin, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, LOW); //active les moteurs (1 seul ici)
  pinMode(X_ENDSTOP_PIN, INPUT_PULLUP);

  // initialisation fin de course
  Serial.println("Initialisation des fins de course");
  finDeCourseX.setDebounceTime(50);

// configuration du moteur X
stepperX.setMaxSpeed(400); //vitesse maximale en pas par seconde
//stepperX.setSpeed(300); //vitesse en pas par seconde (inutile)
stepperX.setAcceleration(300); //acceleration en pas par seconde par seconde²
stepperX.setPinsInverted(false, false, true); //inversion des pins
stepperX.setEnablePin(ENABLE_PIN); //pin d'activation du moteur

Serial.println("Moteurs initialisés");
digitalWrite(ENABLE_PIN, HIGH); //desactive les moteurs
}

void loop() {
  // change la valeur de la led
  ledState = !ledState;
  digitalWrite(ledPin, ledState);
  BougerMoteur(&stepperX, &finDeCourseX, 50000, 1000);
  delay(5000);
}
// fonction bouger
void BougerMoteur(AccelStepper* stepper, ezButton* finDeCourse, int MAX, int distance)
{
  digitalWrite(ENABLE_PIN, LOW); //active les moteurs
  Serial.println("position actuelle : "+String(stepper->currentPosition()));
  long Position_cible = stepper->currentPosition()+distance;
  finDeCourse->loop(); //obligatoire, sinon arrêt après avoir appuyé sur le bouton.
  stepper->moveTo(Position_cible);
  while(stepper->distanceToGo() !=0)
  {
    stepper->run();
  }
  Serial.println("La position du moteur est : "+String(stepper->currentPosition()));
  digitalWrite(ENABLE_PIN, HIGH); //desactive les moteurs
}