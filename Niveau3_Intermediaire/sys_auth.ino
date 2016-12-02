// Inclure les librairies
#include <Keypad.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

int redLED = 12; // Définit la pin de la DEL rouge
int greenLED = 13; // Définit la pin de la DEL verte
#define BACKLIGHT_PIN 3 // Pour l'écran
#define Password_Lenght 7 // Définit la longeur maximum du mot de passe
char Data[Password_Lenght]; // Prépare les données
char Master[Password_Lenght] = "123456"; // Défini le mot de passe
byte data_count = 0, master_count = 0; // Prépare les données
bool Pass_is_good;  // Prépare les données
                    // bool est un vrai ou faux
char customKey; // Prépare la clé que l'utilisateur entrera

byte smiley[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b01010,
  0b00000,
  0b10001,
  0b10001,
  0b01110
}; // Défini un bonhomme sourire

byte bad[8] = {
  0b00000,
  0b01010,
  0b01010,
  0b01010,
  0b00000,
  0b01110,
  0b10001,
  0b10001
}; // Défini une bonhomme grimaçante

const byte ROWS = 4; // Défini le nombre de rangées
const byte COLS = 4; // Défini le nombre de collones
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
}; // Défini la position du clavier
byte rowPins[ROWS] = {6, 7, 8, 9}; // Défini les ports utilisés pour les rangées
byte colPins[COLS] = {2, 3, 4, 5}; // Défini les ports utilisés pour les 

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); // Initialise le clavier numérique

LiquidCrystal_I2C  lcd(0x27, 2, 1, 0, 4, 5, 6, 7); // Initialise l'écran LCD

void setup() // Cette séquence s'exécute une seule fois, au démarrage de l'appareil
{
  lcd.begin (16,2); // Défini la taille de l'écran LCD, 16 par 2
  
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE); // Définit le port pour la lumière de contraste
  lcd.setBacklight(HIGH); // Allume la lumière de contraste

  lcd.createChar(0, smiley); // Initialise le bonhomme sourire
  lcd.createChar(1, bad); // Initialise le bonhomme grimacant

  pinMode(redLED, OUTPUT); // Définit le port de la lumière rouge comme étant une sortie
  pinMode(greenLED, OUTPUT); // Définit le port de la lumière verte comme étant une sortie
  digitalWrite(redLED, LOW); // Éteint la lumière rouge, pour être sûr
  digitalWrite(greenLED, LOW); // Éteint la lumière verte, pour être sûr
}

void loop() // Cette séquence se répête chaque fois qu'elle fini, à l'infini
{
  lcd.setCursor(0,0); // Place le curseur sur la collone 1 de la première ligne
  lcd.print("Entrez votre mot"); // Écrit...
  lcd.setCursor(0,1); // Place le curseur sur la collone 1 de la deuxième ligne
  lcd.print("de passe :"); // Écrit...
  
  customKey = keypad.getKey(); // Récupère les touches pressés du clavier numérique
  if (customKey) // Vérifier qu'il y a une clé
  {
    Data[data_count] = customKey; // Sauvegarde la clé dans une variable
    lcd.setCursor(10+data_count,1); // Déplace le curseur chaque fois qu'une touche est pressé
    lcd.print(Data[data_count]); // Écrit le curseur sur l'écran
    data_count++; // Précise le nombre d'entrée sur le clavier numérique
  }

  if(data_count == Password_Lenght-1) // Si le nombre d'entrée équivaut à la longeur maximale (7-1=6), exécute le suivant
  {
    lcd.clear(); // Efface le texte actuel
    lcd.setCursor(0, 0); // Déplace le curseur
    lcd.print("Le mot de passe"); // Écrit...

    if(!strcmp(Data, Master)) // Si l'entrée équivaut au mot de passe
    {
      lcd.setCursor(0,1); // Déplace le curseur
      lcd.print("est bon!"); // Écrit
      lcd.setCursor(10,1); // Déplace le curseur
      lcd.write(byte(0)); // Affiche le bonhomme sourire
      digitalWrite(greenLED, HIGH); // Allume la lumière verte
      delay(3000); // Attend 3 seconde et...
      digitalWrite(greenLED, LOW); // Ferme la lumière verte
    }
    else // si le mot de passe est pas bone
    {
      lcd.setCursor(0,1); // Déplace le curseur
      lcd.print("est mauvais..."); // Écrit
      lcd.setCursor(15,1); // Déplace le curseur
      lcd.write(byte(1)); // Affiche le bonhomme grimacant
      digitalWrite(redLED, HIGH); // Allume la lumière rouge
      delay(3000);// Attend 3 secondes et ...
      digitalWrite(redLED, LOW); // Ferme la lumière rouge
    }

    lcd.clear(); // Efface l'écran
    clearData(); // Supprime les données temporaire
    
  }
}

void clearData()
{
  while(data_count !=0) // Si le nombre de data_count n'est pas zéro
  {   // Supprime chaque partie, jusqu'à ce qu'il y en ai plus
    Data[data_count--] = 0;
  }
  return; // Retourne au début
}

