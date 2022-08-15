//
//  main.cpp
//  ovning 6A

#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;


//Globala konstanter
const int MAX_TRANSAKTIONER=30;
const int MAX_PERSONER=10;

//Klassdeklarationer
class Transaktion
{
private:
  string datum;
  string typ;
  string namn;
  double belopp;
  int    antal_kompisar;
  string kompisar[MAX_PERSONER];

public:
  Transaktion();
  ~Transaktion();
  string hamtaNamn();
  double hamtaBelopp();
  int    hamtaAntalKompisar();
  bool   finnsKompis(const string &namnet);
  bool   lasIn(istream &is);
  void   skrivUt(ostream &os);
  void   skrivTitel(ostream &os);
};

class Person
{
private:
  string namn;
  double betalat_andras; // ligger ute med totalt
  double skyldig;        // skyldig totalt

public:
  Person();
  Person(const string &namn, double betalat_andras, double skyldig);
  string hamtaNamn();
  double hamtaBetalat();
  double hamtaSkyldig();
  void   skrivUt(ostream &os);
};


class PersonLista
 {
 private:
   int    antal_personer;
   Person personer[MAX_PERSONER];

 public:
   PersonLista();
   ~PersonLista();
   void   laggTill(Person ny_person);
   void   skrivUtOchFixa(ostream &os);
   double summaSkyldig();
   double summaBetalat();
   bool   finnsPerson(const string& namn);

 };

class TransaktionsLista
{
private:
  int         antal_transaktioner;
  Transaktion transaktioner[MAX_TRANSAKTIONER];

public:
  TransaktionsLista();
  ~TransaktionsLista();
  void   lasIn(istream & is);
  void   skrivUt(ostream & os);
  void   laggTill(Transaktion & t);
  double totalKostnad();
  double liggerUteMed(const string &namnet);
  double arSkyldig(const string &namnet);
  PersonLista FixaPersoner();

};


int main(){
    cout << "Startar med att läsa från en fil." << endl;

      TransaktionsLista transaktioner;
      std::ifstream     is("resa.txt");
    
      transaktioner.lasIn(is);
      int operation = 1;
      while (operation != 0)
        {
          cout << endl;
          cout << "Välj i menyn nedan:" << endl;
          cout << "0. Avsluta. Alla transaktioner sparas på fil." << endl;
          cout << "1. Skriv ut information om alla transaktioner." << endl;
          cout << "2. Läs in en transaktion från tangentbordet." << endl;
          cout << "3. Beräkna totala kostnaden." << endl;
          cout << "4. Hur mycket är en viss person skyldig?" << endl;
          cout << "5. Hur mycket ligger en viss person ute med?" << endl;
          cout << "6. Lista alla personer mm och FIXA" << endl;

          cin >> operation;
          cout << endl;
            
          switch (operation)
            {
              case 1:
                {
                  transaktioner.skrivUt(cout);
                  break;
                }
              case 2:
                {
                  Transaktion transaktion;
                  cout << "Ange transaktion i följande format" << endl;
                  transaktion.skrivTitel(cout);
                  transaktion.lasIn(cin);
                  transaktioner.laggTill(transaktion);
                  break;
                }
              case 3:
                {
                  cout << "Den totala kostnanden för resan var "
                       << transaktioner.totalKostnad() << endl;
                  break;
                }
              case 4:
                {
                  cout << "Ange personen: ";
                  string namn;
                  cin >> namn;
                  double ar_skyldig = transaktioner.arSkyldig(namn);
                  if (ar_skyldig == 0.)
                    cout << "Kan inte hitta personen " << namn << endl;
                  else
                    cout << namn << " är skyldig " << ar_skyldig << endl;
                  break;
                }
              case 5:
                {
                  cout << "Ange personen: ";
                  string namn;
                  cin >> namn;
                  double ligger_ute_med = transaktioner.liggerUteMed(namn);
                  if (ligger_ute_med == 0.)
                    cout << "Kan inte hitta personen " << namn << endl;
                  else
                    cout << namn << " ligger ute med " << ligger_ute_med << endl;
                  break;
                }
              case 6:
                {
                  cout << "Nu skapar vi en personarray och reder ut det hela!" << endl;
                  PersonLista lista = transaktioner.FixaPersoner();
                  lista.skrivUtOchFixa(cout);
                  break;
                }
            }
        }

      std::ofstream os("transaktioner.txt");
    transaktioner.skrivUt(os);
    return 0;
}


//Klassimplementering
Transaktion::Transaktion(){
    datum="";
    typ="";
    namn="";
    belopp=0.0;
    antal_kompisar=0;
    for(int i=0; i<MAX_PERSONER;i++){
        kompisar[i]="";
    }
}

Transaktion::~Transaktion(){
    
}

string Transaktion::hamtaNamn(){
    return namn;
}

double Transaktion::hamtaBelopp(){
    return belopp;
}

int Transaktion::hamtaAntalKompisar(){
    return antal_kompisar;
}

bool Transaktion::finnsKompis(const string &namnet){
    bool name_exists=false;
    
    for(int i=0;i<MAX_PERSONER;i++){ //Kolla igenom varje element i arrayen, om det matchar strängen, ändra name_exist till true
        if(kompisar[i]==namnet)
            name_exists=true;
    }
    return name_exists;
}

bool Transaktion::lasIn(istream &is){
    is >> datum >> typ >> namn >> belopp >> antal_kompisar; //Läs in attribut
    
    for(int i=0; i<MAX_PERSONER; i++){ //Nollställ hela arrayen till tomma strängar
        kompisar[i]="";
    }
 
    for(int i=0; i<MAX_PERSONER; i++){ //Lägg till nya namn för varje transaktion i arrayen
        is >> kompisar[i];
    }
    
    return is.eof(); //Returnera om vi har kommit till slutet av filen
}

void Transaktion::skrivUt(ostream &os){
    
    os << datum << "   " << typ << "   " << namn << "   " << belopp << "   " << antal_kompisar << "  "<< endl;
    
    for(int i=0; i<MAX_PERSONER; i++){
        os << kompisar[i] << "   "; //Attribut skrivs ut
    }
    os << endl;
}

void Transaktion::skrivTitel(ostream &os){
    os << "Datum   Typ   Namn   Belopp   Kompisar" << endl;
}

Person::Person(){
    namn="";
    betalat_andras=0.0;
    skyldig=0.0;
}

Person::Person(const string &namn, double betalat_andras, double skyldig)
:namn(namn),betalat_andras(betalat_andras),skyldig(skyldig)
{}

string Person::hamtaNamn(){
    return namn;
}

double Person::hamtaBetalat(){
    return betalat_andras;
}

double Person::hamtaSkyldig(){
    return skyldig;
}

void Person::skrivUt(ostream &os){
    os << namn << " ligger ute med " << betalat_andras << " och är skyldig " << skyldig << ".";
    
    if(betalat_andras < skyldig){
        os << "Skall lägga " << skyldig - betalat_andras << " till potten." << endl;
    }
    else{
        os << "Skall få " << betalat_andras - skyldig << "från potten." << endl;
    }
}

PersonLista::PersonLista(){
    antal_personer=0;
}

PersonLista::~PersonLista(){
    
}

void PersonLista::laggTill(Person ny_person){ //Lägger till ny person till arrayen och ökar värdet på antalet personer.
    personer[antal_personer]=ny_person;
    antal_personer++;
}

void PersonLista::skrivUtOchFixa(ostream &os){
    for(int i=0; i<antal_personer;i++){
        
        personer[i].skrivUt(os); //Skriver ut information om Person-objektet
        
        if(abs(summaBetalat()-summaSkyldig()) < 0.000001){ //Jämför summerad betalning och skuld med differensgräns
            os << "Potten ser rätt ut!" << endl;
        }
        else{
            os << "Ajdå, någon har tagit ut eller betalat för mycket." << endl;
            }
        }

    }

double PersonLista::summaSkyldig(){
    double total_skuld=0.0;
    for(int i=0; i<antal_personer; i++){
        total_skuld+=personer[i].hamtaSkyldig(); //Summerar total skuld
    }
    return total_skuld;
}

double PersonLista::summaBetalat(){
    double total_summa=0.0;
    for(int i=0; i<antal_personer; i++){
        total_summa+=personer[i].hamtaBetalat(); //Summererar tota betalning
    }
    return total_summa;
}

bool PersonLista::finnsPerson(const string& namn){
    bool person_exists=false;
    for(int i=0; i<antal_personer; i++){
        if(personer[i].hamtaNamn()==namn){ //Går igenom namnen på personobjekten och kollar om de mathar med angivna namnet
            person_exists=true;
        }
    }
    return person_exists;
}

TransaktionsLista::TransaktionsLista(){
    antal_transaktioner=0;
}

TransaktionsLista::~TransaktionsLista(){
    
}

void TransaktionsLista::lasIn(istream & is){
    
    Transaktion transak; //Objekt av klassen Transaktion skapas
    
    while(transak.lasIn(is)){ //Data läggs till transaktionslistan så länge outstreamen inte tagit slut
        
        laggTill(transak);
    }
}

void TransaktionsLista::skrivUt(ostream & os){
    os << "Totala antalet transaktioner= " << antal_transaktioner << endl;
    os << "Datum   Typ   Namn   Belopp   Kompisar" << endl;
    for(int i=0; i<antal_transaktioner; i++){ //Transaktionslista skrivs ut
        transaktioner[i].skrivUt(os);
    }
}

void TransaktionsLista::laggTill(Transaktion & transak){
    transaktioner[antal_transaktioner]=transak; //Lägger till ny transaktion i arrayen
    antal_transaktioner++;
}

double TransaktionsLista::totalKostnad(){
    
    double total_sum=0.0;
    
    for(int i=0; i<antal_transaktioner; i++){
        total_sum+=transaktioner[i].hamtaBelopp();
    }
    return total_sum;
}

double TransaktionsLista::liggerUteMed(const string &namnet){
    double summa = 0.0;
    for(int i = 0; i < antal_transaktioner; i++){
        //Om namn hittas bland transaktionerna ökas värdet på summan med avseende på hur många kompisar som kostaden delades av
          if (transaktioner[i].hamtaNamn() == namnet){
          summa += transaktioner[i].hamtaBelopp() *
                   (1.0 - 1.0 / (transaktioner[i].hamtaAntalKompisar() + 1));
          }
    }
    return summa;
}

double TransaktionsLista::arSkyldig(const string &namnet){
    double skuld=0.0;
    for(int i=0; i < antal_transaktioner; i++){
        if(transaktioner[i].finnsKompis(namnet)){ //Ökar värdet på skulden om personen hittas i transaktionerna
            skuld += (transaktioner[i].hamtaBelopp()/ (transaktioner[i].hamtaAntalKompisar()+1));
        }
    }
    return skuld;
}

PersonLista TransaktionsLista::FixaPersoner(){
    PersonLista p_list;
    string p_namn;
    double p_betalt;
    double p_skyldig;
    
    for(int i=0; i<antal_transaktioner; i++){ //Itererar över alla transaktioner
        
        if(!p_list.finnsPerson(transaktioner[i].hamtaNamn())){
            //Ger värde till namn, betalat och skuld om personen inte finns
            p_namn=transaktioner[i].hamtaNamn();
            p_betalt=liggerUteMed(p_namn);
            p_skyldig=arSkyldig(p_namn);
            
            Person p_ny(p_namn,p_betalt,p_skyldig); //Skapar ny person med nyligen värdesatta variabler som attribut
            p_list.laggTill(p_ny); //Personen läggs till i personlistan
        }
    }
    return p_list;
}

