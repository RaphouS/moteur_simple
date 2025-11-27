// main.cpp 3 rideaux
#include <Arduino.h>
#include <ezButton.h>
#include <AccelStepper.h>




//definition des pin pour les moteurs
#define ENABLE_PIN 8 /* Enable pin for all stepper outputs */ 
#define X_DIR 7 /* Direction-Pin for X-axis */ 
#define X_STEP 4 /* Step-Pin for X-axis */ 
#define X_ENDSTOP_PIN 11 /* Limit switch for X-axis */


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
void BougerMoteurAvecFinDeCourse(AccelStepper* stepper, ezButton* finDeCourse, int distance);



// ajout d'une interaction avec la console série
//----------------------------------------------------------------------------------------------------------------------------

// Attention, on ne voit pas le texte tapé tant que l'on n'appuie pas sur "Entrée"


enum TerminalState { Wait_command, Idle };
TerminalState terminalState = Idle;

void prompt_commande()
{
  Serial.println("Entrer une commande : h pour horaire, a pour anti-horaire, suivi de la distance en pas (ex: h1000)");
  terminalState = Wait_command;
}

void check_commandes()
{
  if (terminalState == Idle)
  {
    prompt_commande();
    return;
  }

  if (Serial.available() >0) 
  {
    Serial.setTimeout(5000); // timeout en millisecondes (ici 5 s)
    String reponse = Serial.readStringUntil('\n');  // Lire la ligne saisie
    Serial.println("Commande reçue : " + reponse);
    reponse.trim();  // Supprime espaces et saut de ligne
    if (reponse.length() > 1)
    {
      char cmd = reponse.charAt(0);
      int distance = reponse.substring(1).toInt();
      if (cmd == 'h') //horaire
      {
        BougerMoteurAvecFinDeCourse(&stepperX, &finDeCourseX, stepperX.currentPosition() + distance);
      }
      else if (cmd == 'a') //anti-horaire
      {
        BougerMoteurAvecFinDeCourse(&stepperX, &finDeCourseX, stepperX.currentPosition() - distance);
      }
      else if (cmd == 's') //stop
      {
        stepperX.stop();
        stepperX.runToPosition();
        Serial.println("Mouvement arrêté. Position actuelle : " + String(stepperX.currentPosition()));
      }
      else
      {
        Serial.println("Commande inconnue");
      }
    }
    else
    {
      Serial.println("Commande trop courte");
    }
    prompt_commande();
  }
} 

//----------------------------------------------------------------------------------------------------------------------------


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
stepperX.setMaxSpeed(100); //vitesse maximale en pas par seconde
//stepperX.setSpeed(300); //vitesse en pas par seconde (inutile)
stepperX.setAcceleration(500); //acceleration en pas par seconde par seconde²
stepperX.setPinsInverted(false, false, true); //inversion des pins
stepperX.setEnablePin(ENABLE_PIN); //pin d'activation du moteur

Serial.println("Moteurs initialisés");
digitalWrite(ENABLE_PIN, HIGH); //desactive les moteurs
terminalState = Idle;
prompt_commande();

}

void loop() {
  // change la valeur de la led
  ledState = !ledState;
  /*digitalWrite(ledPin, ledState);
  //BougerMoteur(&stepperX, &finDeCourseX, 10000, 1000);
  BougerMoteurAvecFinDeCourse(&stepperX, &finDeCourseX, 10000);
  Serial.println("Attente 5 secondes avant le prochain mouvement");
  delay(5000);
  Serial.println("Retour à la position initiale");
  BougerMoteurAvecFinDeCourse(&stepperX, &finDeCourseX, 0);
  Serial.println("Attente 5 secondes avant le prochain mouvement");
  delay(5000);
  */

// gestion des commandes série
  check_commandes();
  delay(2);
}


// fonction bouger
void BougerMoteur(AccelStepper* stepper, ezButton* finDeCourse, int MAX, int distance)
{
    Serial.println("position actuelle : "+String(stepper->currentPosition()));
  long Position_cible = stepper->currentPosition()+distance;
  finDeCourse->loop(); //obligatoire, sinon arrêt après avoir appuyé sur le bouton.
  if (Position_cible > MAX || finDeCourse->isPressed())
  {
    Serial.println("Fin de course atteinte ou position maximale atteinte");
    return;
  }
  digitalWrite(ENABLE_PIN, LOW); //active les moteurs
  stepper->moveTo(Position_cible);
  while(stepper->distanceToGo() !=0)
  {
    stepper->run();
  }
  Serial.println("La position du moteur est : "+String(stepper->currentPosition()));
  digitalWrite(ENABLE_PIN, HIGH); //desactive les moteurs
}

// fonction bouger avec détectiotn de fin de course
void BougerMoteurAvecFinDeCourse(AccelStepper* stepper, ezButton* finDeCourse, int Position_cible)
{
  Serial.println("position actuelle : "+String(stepper->currentPosition()));

  finDeCourse->loop(); //obligatoire, sinon arrêt après avoir appuyé sur le bouton.

  digitalWrite(ENABLE_PIN, LOW); //active les moteurs
  stepper->moveTo(Position_cible);
  while(stepper->distanceToGo() !=0)
  {
    finDeCourse->loop(); //obligatoire, sinon arrêt après avoir appuyé sur le bouton.
    if (finDeCourse->isPressed())
    {
      Serial.println("Fin de course atteinte pendant le mouvement");
      stepper->stop();
      stepper->runToPosition(); //avance jusqu'à la position d'arrêt
      break; //sortir de la boucle while
    }
    stepper->run();
  }
  Serial.println("La position du moteur est : "+String(stepper->currentPosition()));
  digitalWrite(ENABLE_PIN, HIGH); //desactive les moteurs
}
