#include <SFML/Graphics.hpp>
#include <iostream>
#include<fstream>
#include <sstream>
#include <SFML/Audio.hpp>
#include <random>


int numarPieseJucator1 = 8;
int numarPieseJucator2 = 8;
int numarPieseJucator1Imaginar = 8;
int numarPieseJucator2Imaginar = 8;
const float DimensiunePatrat = 135.0f;
const int NumarLinii = 8;
const int NumarColoane = 8;
int culoareTura = 1;
int TurePace =8;
int Ture = 0;
bool piesaSelectata = false;
int muzica = 1;
int sunet = 1;
int modJoc = 0;
int Start = 0;
int piesaSelectataX = -1;
int piesaSelectataY = -1;
int xDestinatie = -1;
int yDestinatie = -1;
const sf::Color FundalGri = sf::Color(81, 80, 76);
int castigator = 0;

sf::RenderWindow window(sf::VideoMode(1920, 1080), "Razboi in 8!", sf::Style::None);
sf::Texture TexturaPiesaAlba;
sf::Texture TexturaPiesaNeagra;
sf::Sprite PiesaAlbaSprite;
sf::Sprite PiesaNeagraSprite;
sf::Sprite CeasASprite;
sf::Sprite CeasNSprite;
sf::Texture TexturaCeasA;
sf::Texture TexturaCeasN;
sf::Sprite PanouSprite;
sf::Texture TexturaPanou;
sf::Text timerText;
sf::SoundBuffer bufferSelectieMod;
sf::Sound sunetSelectieMod;
sf::Texture TexturaCastigJucator1;
sf::Texture TexturaCastigJucator2;
sf::Texture TexturaCastigJucator;
sf::Texture TexturaPierdutJucator;
std::random_device rd;
std::mt19937 generator(rd());
std::uniform_int_distribution<int> distribution(0, 32);
sf::Music music1;
sf::SoundBuffer buffer1;
sf::SoundBuffer buffer2;
std::ofstream fisierO("tabla.txt");
std::ifstream fisierI("tabla.txt");
sf::Sound sound1;
sf::Sound sound2;
sf::Clock clockA;
sf::Clock clockB;
sf::Time remainingTimeA = sf::seconds(300);  // 5 minute
sf::Time remainingTimeB = sf::seconds(300);  // 5 minute
sf::Time elapsedInCurrentTurnA;
sf::Time elapsedInCurrentTurnB;


struct scorul {
    int scor;
    int YInitial;
    int XInitial;
    int DestinatieX;
    int DestinatieY;
}mutare[32];
int tabla[NumarLinii][NumarColoane] = {
    {0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 2, 0, 2, 0, 2, 0, 2},
    {2, 0, 2, 0, 2, 0, 2, 0}
};
int tablaImg[8][8];



// Funcție pentru pornirea cronometrului pentru jucătorul curent
void NumararePiese() {
    numarPieseJucator1 = 0;
    numarPieseJucator2 = 0;
    for (int i = 0; i < NumarLinii; i++) {
        for (int j = 0; j < NumarColoane; j++) {
            if (tabla[j][i] == 1) {
                numarPieseJucator1++;
            }
            else if (tabla[j][i] == 2) {
                numarPieseJucator2++;
            }
        }
    }

}

void NumararePieseImaginar() {
    numarPieseJucator1Imaginar = 0;
    numarPieseJucator2Imaginar = 0;
    for (int i = 0; i < NumarLinii; i++) {
        for (int j = 0; j < NumarColoane; j++) {
            if (tablaImg[j][i] == 1) {
                numarPieseJucator1Imaginar++;
            }
            else if (tablaImg[j][i] == 2) {
                numarPieseJucator2Imaginar++;
            }
        }
    }

}

void copiazaTabla() {
    for (int i = 0; i < NumarLinii; i++) {
        for (int j = 0; j < NumarColoane; j++)
            tablaImg[i][j] = tabla[i][j];
    }
}

void startClock() {
    if (culoareTura == 1) {
        clockA.restart();
        if (Start == 1) {
            elapsedInCurrentTurnB = clockB.getElapsedTime();
            remainingTimeB -= elapsedInCurrentTurnB;
        }
    }
    else {
        clockB.restart();
        if (Start == 1) {
            elapsedInCurrentTurnA = clockA.getElapsedTime();
            remainingTimeA -= elapsedInCurrentTurnA;
        }
    }
}

void timerA(sf::RenderWindow& window) {

    if (Start == 1 && culoareTura == 1) {
        elapsedInCurrentTurnA = clockA.getElapsedTime();
        remainingTimeA -= elapsedInCurrentTurnA;
        clockA.restart();
    }

    // Converteste timpul rămas la minute și secunde
    int minutes = static_cast<int>(remainingTimeA.asSeconds()) / 60;
    int seconds = static_cast<int>(remainingTimeA.asSeconds()) % 60;

    // Actualizează textul doar dacă timpul nu a expirat
    if (remainingTimeA > sf::seconds(0)) {


        sf::Font font;
        if (!font.loadFromFile("arial.ttf")) {
            // Tratează eroarea dacă nu poate încărca fontul
            std::cerr << "Eroare la incarcarea fontului!\n";
            return;
        }

        timerText.setFont(font);
        timerText.setCharacterSize(100);
        timerText.setFillColor(sf::Color(81, 80, 76));
        timerText.setPosition(1230.0f, 100.0f);

        // Construiește șirul de caractere pentru afișarea timpului rămas
        std::ostringstream ss;
        if (seconds >= 10)
            ss << minutes << ":" << seconds << "";
        else
            ss << minutes << ":0" << seconds << "";
        timerText.setString(ss.str());


        window.draw(timerText);
    }
}

void timerB(sf::RenderWindow& window) {

    if (Start == 1 && culoareTura == 2) {
        elapsedInCurrentTurnB = clockB.getElapsedTime();
        remainingTimeB -= elapsedInCurrentTurnB;
        clockB.restart();
    }

    // Converteste timpul rămas la minute și secunde
    int minutes = static_cast<int>(remainingTimeB.asSeconds()) / 60;
    int seconds = static_cast<int>(remainingTimeB.asSeconds()) % 60;

    // Actualizează textul doar dacă timpul nu a expirat
    if (remainingTimeB > sf::seconds(0)) {


        sf::Font font;
        if (!font.loadFromFile("arial.ttf")) {
            // Tratează eroarea dacă nu poate încărca fontul
            std::cerr << "Eroare la incarcarea fontului!\n";
            return;
        }

        timerText.setFont(font);
        timerText.setCharacterSize(100);
        timerText.setFillColor(sf::Color::White);
        timerText.setPosition(1600.0f, 100.0f);

        // Construiește șirul de caractere pentru afișarea timpului rămas
        std::ostringstream ss;
        if (seconds >= 10)
            ss << minutes << ":" << seconds << "";
        else
            ss << minutes << ":0" << seconds << "";
        timerText.setString(ss.str());


        window.draw(timerText);
    }
}

void switchPlayer() {
    if (culoareTura == 1) {
        elapsedInCurrentTurnA = clockA.getElapsedTime();
        remainingTimeA -= elapsedInCurrentTurnA;
        culoareTura = 2;
        clockB.restart();
    }
    else {
        elapsedInCurrentTurnB = clockB.getElapsedTime();
        remainingTimeB -= elapsedInCurrentTurnB;
        culoareTura = 1;
        clockA.restart();
    }
    Start = 0;
}

void initializeazaUI() {
    if (!TexturaCeasA.loadFromFile("Texturi/CeasA.png") || !TexturaCeasN.loadFromFile("Texturi/CeasN.png") || !TexturaPanou.loadFromFile("Texturi/Panou.png")) {
        throw std::runtime_error("Eroare la incarcarea texturilor pentru ceas!");
    }

    // Setăm pozițiile pentru sprite-urile de ceas
    CeasASprite.setTexture(TexturaCeasA);
    CeasNSprite.setTexture(TexturaCeasN);
    PanouSprite.setTexture(TexturaPanou);

    CeasASprite.setPosition(DimensiunePatrat * NumarLinii + 100, 100);
    CeasNSprite.setPosition(DimensiunePatrat * NumarLinii + 475, 100);
    PanouSprite.setPosition(DimensiunePatrat * NumarLinii + 25, 325);

    // Adăugăm sprite-urile la fereastra
    sf::RectangleShape fundal;
    fundal.setSize(sf::Vector2f(window.getSize().x - DimensiunePatrat * NumarLinii, window.getSize().y));
    fundal.setPosition(DimensiunePatrat * NumarLinii, 0);
    fundal.setFillColor(FundalGri);
    window.draw(fundal);
    window.draw(CeasASprite);
    window.draw(CeasNSprite);
    window.draw(PanouSprite);
    timerA(window);
    timerB(window);


}

void initializeazaJoc() {
    window.setFramerateLimit(60);
    if (!music1.openFromFile("Sunet/melodie.mp3"))
    {
        throw std::runtime_error("incarcare muzica eroare");
    }
    music1.setVolume(20);
    if (!buffer1.loadFromFile("Sunet/click.mp3") || !buffer2.loadFromFile("Sunet/move.mp3"))
    {
        throw std::runtime_error("incarcare sunet eroare");
    }
    if (!TexturaPiesaAlba.loadFromFile("Texturi/PiesaAlba.png") || !TexturaPiesaNeagra.loadFromFile("Texturi/PiesaNeagra.png")) {
        throw std::runtime_error("Eroare la incarcarea texturilor!");
    }
    if (!bufferSelectieMod.loadFromFile("Sunet/IncepereJoc.wav")) {
        throw std::runtime_error("Eroare la încărcarea sunetului de selecție a modului de joc!");
    }
    if (!TexturaCastigJucator1.loadFromFile("Texturi/AlbW.png") || !TexturaCastigJucator2.loadFromFile("Texturi/NegruW.png")) {
        throw std::runtime_error("Eroare la incarcarea texturilor pentru ecranul de castig!");
    }
    if (!TexturaCastigJucator.loadFromFile("Texturi/Castigat.png") || !TexturaPierdutJucator.loadFromFile("Texturi/Pierdut.png")) {
        throw std::runtime_error("Eroare la incarcarea texturilor pentru ecranul de castig!");
    }
    sunetSelectieMod.setBuffer(bufferSelectieMod);
    sound1.setBuffer(buffer1);
    sound2.setBuffer(buffer2);

    PiesaAlbaSprite.setTexture(TexturaPiesaAlba);
    PiesaNeagraSprite.setTexture(TexturaPiesaNeagra);

}

bool mutareValida(int piesaSelectataX, int piesaSelectataY, int xDestinatie, int yDestinatie) {

    if (piesaSelectataX < 0 || piesaSelectataX >= NumarLinii || piesaSelectataY < 0 || piesaSelectataY >= NumarColoane ||
        xDestinatie < 0 || xDestinatie >= NumarLinii || yDestinatie < 0 || yDestinatie >= NumarColoane) {
        return false;  // Verificăm dacă coordonatele sunt în interiorul tablei
    }

    // Verificăm dacă destinația este goală
    if (tabla[yDestinatie][xDestinatie] == 0) {
        if (tabla[piesaSelectataY][piesaSelectataX] == 1) {
            // Mutare normală în față pe diagonală
            if (yDestinatie == piesaSelectataY + 1 && (xDestinatie == piesaSelectataX + 1 || xDestinatie == piesaSelectataX - 1)) {
                return true;
            }
            if (yDestinatie == piesaSelectataY - 1 && (xDestinatie == piesaSelectataX + 1 || xDestinatie == piesaSelectataX - 1)) {
                return true;
            }
        }
        if (tabla[piesaSelectataY][piesaSelectataX] == 2) {
            // Mutare normală în față pe diagonală
            if (yDestinatie == piesaSelectataY - 1 && (xDestinatie == piesaSelectataX + 1 || xDestinatie == piesaSelectataX - 1)) {
                return true;
            }
            if (yDestinatie == piesaSelectataY + 1 && (xDestinatie == piesaSelectataX + 1 || xDestinatie == piesaSelectataX - 1)) {
                return true;
            }
        }
    }

    return false;
}
//eliminare piese blocate
void pieseBlocate() {
    for (int i = 0; i < NumarLinii; i++) {
        for (int j = 0; j < NumarColoane; j++) {
            if (tabla[j][i] != 0) {
                // Cazul 1: Piesa în colțul dreapta sus și blocată de piesa în G7
                if (i == 7 && j == 0 && tabla[j + 1][i - 1] != 0 && TurePace <= 0) {
                    tabla[j][i] = 0;
                    if (sunet == 1)
                    {
                        sound2.play();
                    }
                }
                // Cazul 2: Piesa în colțul stânga jos și blocată de piesa în B2
                else if (i == 0 && j == 7 && tabla[j - 1][i + 1] != 0 && TurePace <= 0) {
                    tabla[j][i] = 0;
                    if (sunet == 1)
                    {
                        sound2.play();
                    }
                }
                // Cazul 3: Piesa la marginea de jos și blocată de cele două piese adiacente
                else if (j == 7 && tabla[j - 1][i + 1] != 0 && tabla[j - 1][i - 1] != 0 && TurePace <= 0) {
                    tabla[j][i] = 0;
                    if (sunet == 1)
                    {
                        sound2.play();
                    }
                }
                // Cazul 4: Piesa la marginea de sus și blocată de cele două piese adiacente
                else if (j == 0 && tabla[j + 1][i + 1] != 0 && tabla[j + 1][i - 1] != 0 && TurePace <= 0) {
                    tabla[j][i] = 0;
                    if (sunet == 1)
                    {
                        sound2.play();
                    }
                }
                // Cazul 5: Piesa la marginea din stânga și blocată de două piese adiacente
                else if (j != 7 && i == 0 && tabla[j - 1][i + 1] != 0 && tabla[j + 1][i + 1] != 0) {
                    tabla[j][i] = 0;
                    if (sunet == 1)
                    {
                        sound2.play();
                    }
                }
                // Cazul 6: Piesa la marginea din dreapta și blocată de piesa în G7
                else if (j != 0 && i == 7 && tabla[j - 1][i - 1] != 0 && tabla[j + 1][i - 1] != 0) {
                    tabla[j][i] = 0;
                    if (sunet == 1)
                    {
                        sound2.play();
                    }
                }
                // Cazul 7: Piesa în interiorul tablei și blocată diagonal de cele patru piese adiacente
                else if (i > 0 && i < NumarLinii - 1 && j > 0 && j < NumarColoane - 1 &&
                    tabla[j - 1][i - 1] != 0 && tabla[j - 1][i + 1] != 0 &&
                    tabla[j + 1][i - 1] != 0 && tabla[j + 1][i + 1] != 0) {
                    tabla[j][i] = 0;
                    if (sunet == 1)
                    {
                        sound2.play();
                    }
                }
            }
        }
    }
}
//verificare daca mutare blocheaza piese 
void pieseBlocateImg() {
    for (int i = 0; i < NumarLinii; i++) {
        for (int j = 0; j < NumarColoane; j++) {
            if (tablaImg[j][i] != 0) {
                // Cazul 1: Piesa în colțul dreapta sus și blocată de piesa în G7
                if (i == 7 && j == 0 && tablaImg[j + 1][i - 1] != 0 && TurePace <= 0) {
                    tablaImg[j][i] = 0;
                }
                // Cazul 2: Piesa în colțul stânga jos și blocată de piesa în B2
                else if (i == 0 && j == 7 && tablaImg[j - 1][i + 1] != 0 && TurePace <= 0) {
                    tablaImg[j][i] = 0;
                }
                // Cazul 3: Piesa la marginea de jos și blocată de cele două piese adiacente
                else if (j == 7 && tablaImg[j - 1][i + 1] != 0 && tablaImg[j - 1][i - 1] != 0 && TurePace <= 0) {
                    tablaImg[j][i] = 0;
                }
                // Cazul 4: Piesa la marginea de sus și blocată de cele două piese adiacente
                else if (j == 0 && tablaImg[j + 1][i + 1] != 0 && tablaImg[j + 1][i - 1] != 0 && TurePace <= 0) {
                    tablaImg[j][i] = 0;
                }
                // Cazul 5: Piesa la marginea din stânga și blocată de două piese adiacente
                else if (j != 7 && i == 0 && tablaImg[j - 1][i + 1] != 0 && tablaImg[j + 1][i + 1] != 0) {
                    tablaImg[j][i] = 0;
                }
                // Cazul 6: Piesa la marginea din dreapta și blocată de piesa în G7
                else if (j != 0 && i == 7 && tablaImg[j - 1][i - 1] != 0 && tablaImg[j + 1][i - 1] != 0) {
                    tablaImg[j][i] = 0;
                }
                // Cazul 7: Piesa în interiorul tablei și blocată diagonal de cele patru piese adiacente
                else if (i > 0 && i < NumarLinii - 1 && j > 0 && j < NumarColoane - 1 &&
                    tablaImg[j - 1][i - 1] != 0 && tablaImg[j - 1][i + 1] != 0 &&
                    tablaImg[j + 1][i - 1] != 0 && tablaImg[j + 1][i + 1] != 0) {
                    tablaImg[j][i] = 0;
                }
            }
        }
    }
}

void deseneazaPiesa(int x, int y, sf::Sprite& sprite, bool selected) {
    float posX = x * DimensiunePatrat;
    float posY = y * DimensiunePatrat;

    // Obținem poziția cursorului
    float cursorX = sf::Mouse::getPosition(window).x - DimensiunePatrat / 2;
    float cursorY = sf::Mouse::getPosition(window).y - DimensiunePatrat / 2;

    // Dacă piesa este selectată și mouse-ul este apăsat, actualizăm poziția piesei
    if (selected && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        posX = std::max(0.0f, std::min(window.getSize().x - DimensiunePatrat, cursorX));
        posY = std::max(0.0f, std::min(window.getSize().y - DimensiunePatrat, cursorY));

        // Asigurăm că piesa nu trece în afara tablei în partea dreaptă
        posX = std::min(posX, (NumarLinii - 1) * DimensiunePatrat);
    }
    else {
        posX += (DimensiunePatrat - sprite.getGlobalBounds().width) / 2;
        posY += (DimensiunePatrat - sprite.getGlobalBounds().height) / 2;
    }

    sprite.setPosition(posX, posY);
    window.draw(sprite);
}

void afiseazaPiesele() {
    for (int i = 0; i < NumarLinii; i++) {
        for (int j = 0; j < NumarColoane; j++) {
            if (tabla[j][i] == 1 || tabla[j][i] == 2) {
                bool isSelected = (tabla[j][i] == 1 && piesaSelectataX == i && piesaSelectataY == j) ||
                    (tabla[j][i] == 2 && piesaSelectataX == i && piesaSelectataY == j);

                if (tabla[j][i] == 1) {
                    deseneazaPiesa(i, j, PiesaAlbaSprite, isSelected);
                }
                else {
                    deseneazaPiesa(i, j, PiesaNeagraSprite, isSelected);
                }
            }
        }
    }
}
//gestionare mutare
void mutaPiesa(int xDestinatie, int yDestinatie) {
    // Verificăm dacă mutarea este validă
    if (mutareValida(piesaSelectataX, piesaSelectataY, xDestinatie, yDestinatie)) {
        // Verificăm dacă există o piesă la destinație


        // Actualizăm tabla de joc

        tabla[yDestinatie][xDestinatie] = tabla[piesaSelectataY][piesaSelectataX];
        tabla[piesaSelectataY][piesaSelectataX] = 0;

        if (sunet == 1)
        {
            sound2.play();
        }
        Ture++;
        switchPlayer();
        TurePace--;
        Start = 1;
        pieseBlocate();
        // Eliberăm piesa selectată
        piesaSelectataX = -1;
        piesaSelectataY = -1;
    }
    else {
        // Dacă mutarea nu este validă, revenim la poziția inițială
        piesaSelectataX = -1;
        piesaSelectataY = -1;
    }

}

int mutareValidaCal(int xInitial, int yInitial, int xDestinatie, int yDestinatie)
{
    if (xInitial < 0 || xInitial >= NumarLinii || yInitial < 0 || piesaSelectataY >= yInitial ||
        xDestinatie < 0 || xDestinatie >= NumarLinii || yDestinatie < 0 || yDestinatie >= NumarColoane) {
        return false;  // Verificăm dacă coordonatele sunt în interiorul tablei
    }
    if (tablaImg[xDestinatie][yDestinatie] == 2)
        return false;
    if (tablaImg[xDestinatie][yDestinatie] == 1)
        return false;
    return true;
}
//gestionare mutare calculator
int mutaPiesaCalculator(int xInitial, int yInitial, int xDestinatie, int yDestinatie) {
    // Verificăm dacă coordonatele sunt în limite și dacă destinația este liberă
    if (xInitial >= 0 && xInitial < NumarLinii && yInitial >= 0 && yInitial < NumarColoane &&
        xDestinatie >= 0 && xDestinatie < NumarLinii && yDestinatie >= 0 && yDestinatie < NumarColoane &&
        tablaImg[xInitial][yInitial] == 2 && tablaImg[xDestinatie][yDestinatie] == 0) {

        // Verificăm dacă mutarea este pe diagonală
        if (abs(xDestinatie - xInitial) == 1 && abs(yDestinatie - yInitial) == 1) {
            tablaImg[xDestinatie][yDestinatie] = 2; // Muta piesa
            tablaImg[xInitial][yInitial] = 0; // Elimina piesa de la pozitia initiala
            return 1;
        }
    }
    return 0;
}

//algoritmul de calcul al unei mutari bune
void GestionareMutareNegru() {
    int cnt = 0;
    int scormax = 0;
    copiazaTabla();

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            if (tablaImg[i][j] == 2) {
                if (mutaPiesaCalculator(i, j, i - 1, j - 1))
                {
                    NumararePiese();
                    pieseBlocateImg();
                    NumararePieseImaginar();
                    mutare[cnt].scor = (numarPieseJucator1 - numarPieseJucator1Imaginar) - (numarPieseJucator2 - numarPieseJucator2Imaginar);
                    mutare[cnt].XInitial = i;
                    mutare[cnt].YInitial = j;
                    mutare[cnt].DestinatieX = i - 1;
                    mutare[cnt].DestinatieY = j - 1;
                    if (mutare[cnt].scor >= scormax) {
                        scormax = mutare[cnt].scor;

                    }
                    cnt++;
                    copiazaTabla();

                }
                if (mutaPiesaCalculator(i, j, i - 1, j + 1))
                {
                    NumararePiese();
                    pieseBlocateImg();
                    NumararePieseImaginar();
                    mutare[cnt].scor = (numarPieseJucator1 - numarPieseJucator1Imaginar) - (numarPieseJucator2 - numarPieseJucator2Imaginar);
                    mutare[cnt].XInitial = i;
                    mutare[cnt].YInitial = j;
                    mutare[cnt].DestinatieX = i - 1;
                    mutare[cnt].DestinatieY = j + 1;
                    if (mutare[cnt].scor >= scormax)
                        scormax = mutare[cnt].scor;
                    cnt++;
                    copiazaTabla();

                }
                if (mutaPiesaCalculator(i, j, i + 1, j - 1))
                {

                    NumararePiese();
                    pieseBlocateImg();
                    NumararePieseImaginar();
                    mutare[cnt].scor = (numarPieseJucator1 - numarPieseJucator1Imaginar) - (numarPieseJucator2 - numarPieseJucator2Imaginar);
                    mutare[cnt].XInitial = i;
                    mutare[cnt].YInitial = j;
                    mutare[cnt].DestinatieX = i + 1;
                    mutare[cnt].DestinatieY = j - 1;
                    if (mutare[cnt].scor >= scormax)
                        scormax = mutare[cnt].scor;
                    cnt++;
                    copiazaTabla();
                }
                if (mutaPiesaCalculator(i, j, i + 1, j + 1))
                {

                    NumararePiese();
                    pieseBlocateImg();
                    NumararePieseImaginar();
                    mutare[cnt].scor = (numarPieseJucator1 - numarPieseJucator1Imaginar) - (numarPieseJucator2 - numarPieseJucator2Imaginar);
                    mutare[cnt].XInitial = i;
                    mutare[cnt].YInitial = j;
                    mutare[cnt].DestinatieX = i + 1;
                    mutare[cnt].DestinatieY = j + 1;
                    if (mutare[cnt].scor >= scormax)
                        scormax = mutare[cnt].scor;
                    cnt++;
                    copiazaTabla();
                }
            }
        }
    // Dacă există mutări posibile, alegem una aleatorie dintre cele mai bune
    if (Ture == 1)
    {
        if (sunet == 1)
        {
            sound2.play();
        }
        tabla[6][7] = 0;
        tabla[5][6] = 2;
    }
    else if (Ture == 2)
    {
        if (sunet == 1)
        {
            sound2.play();
        }
        tabla[6][5] = 0;
        tabla[5][4] = 2;
    }
    else if (Ture == 3)
    {
        if (sunet == 1)
        {
            sound2.play();
        }
        tabla[6][1] = 0;
        tabla[5][0] = 2;
    }
    if (Ture > 3) {
        int ales = 0;
        int randomNumber;
        std::uniform_int_distribution<int> distribution(0, cnt - 1);
        while (ales == 0)
        {

            randomNumber = distribution(generator);



            if (mutare[randomNumber].scor == scormax)
            {
                ales = 1;
                if (sunet == 1)
                {
                    sound2.play();
                }
                tabla[mutare[randomNumber].XInitial][mutare[randomNumber].YInitial] = 0;
                tabla[mutare[randomNumber].DestinatieX][mutare[randomNumber].DestinatieY] = 2;
            }

        }
    }
}

void proceseazaEvenimente() {

    sf::Event event;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            modJoc = 0; // Setează modul jocului la 0 pentru a reveni la meniu
            return;
        }
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            int mouseX = event.mouseButton.x / DimensiunePatrat;
            int mouseY = event.mouseButton.y / DimensiunePatrat;

            // Dacă avem deja o piesă selectată și se face clic pe o piesă validă, schimbăm piesa selectată
            if (piesaSelectata && mouseX >= 0 && mouseX < NumarLinii && mouseY >= 0 && mouseY < NumarColoane) {
                if ((culoareTura == 1 && tabla[mouseY][mouseX] == 1) || (culoareTura == 2 && tabla[mouseY][mouseX] == 2)) {
                    piesaSelectataX = mouseX;
                    piesaSelectataY = mouseY;

                }
            }
            // Dacă nu avem o piesă selectată, încercăm să selectăm una
            else if (!piesaSelectata && mouseX >= 0 && mouseX < NumarLinii && mouseY >= 0 && mouseY < NumarColoane) {
                if ((culoareTura == 1 && tabla[mouseY][mouseX] == 1) || (culoareTura == 2 && tabla[mouseY][mouseX] == 2)) {
                    piesaSelectataX = mouseX;
                    piesaSelectataY = mouseY;
                    piesaSelectata = true;

                }
            }
        }
        else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            if (piesaSelectata) {
                int mouseX = event.mouseButton.x / DimensiunePatrat;
                int mouseY = event.mouseButton.y / DimensiunePatrat;

                // Dacă faci clic drept pe pătrat, încercăm să mutăm piesa
                if (mouseX >= 0 && mouseX < NumarLinii && mouseY >= 0 && mouseY < NumarColoane && tabla[mouseY][mouseX] == 0) {
                    xDestinatie = mouseX;
                    yDestinatie = mouseY;
                    mutaPiesa(xDestinatie, yDestinatie);
                    piesaSelectata = false;
                }
            }
        }
    }

    if (piesaSelectata) {
        PiesaAlbaSprite.setPosition(sf::Mouse::getPosition(window).x - DimensiunePatrat / 2, sf::Mouse::getPosition(window).y - DimensiunePatrat / 2);
    }
}

void ProceseazaEvenimenteCalculator() {
    sf::Event event;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            modJoc = 0; // Setează modul jocului la 0 pentru a reveni la meniu
            return;
        }
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            int mouseX = event.mouseButton.x / DimensiunePatrat;
            int mouseY = event.mouseButton.y / DimensiunePatrat;

            // Dacă avem deja o piesă selectată și se face clic pe o piesă validă, schimbăm piesa selectată
            if (piesaSelectata && mouseX >= 0 && mouseX < NumarLinii && mouseY >= 0 && mouseY < NumarColoane) {
                if ((culoareTura == 1 && tabla[mouseY][mouseX] == 1)) {
                    piesaSelectataX = mouseX;
                    piesaSelectataY = mouseY;
                }
            }
            // Dacă nu avem o piesă selectată, încercăm să selectăm una
            else if (!piesaSelectata && mouseX >= 0 && mouseX < NumarLinii && mouseY >= 0 && mouseY < NumarColoane) {
                if ((culoareTura == 1 && tabla[mouseY][mouseX] == 1)) {
                    piesaSelectataX = mouseX;
                    piesaSelectataY = mouseY;
                    piesaSelectata = true;
                }
            }
        }
        if (culoareTura == 2) {

            GestionareMutareNegru();
            culoareTura = 1;
            TurePace--;
            pieseBlocate();
        }
        else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            if (piesaSelectata) {
                int mouseX = event.mouseButton.x / DimensiunePatrat;
                int mouseY = event.mouseButton.y / DimensiunePatrat;

                // Dacă faci clic drept pe pătrat, încercăm să mutăm piesa
                if (mouseX >= 0 && mouseX < NumarLinii && mouseY >= 0 && mouseY < NumarColoane && tabla[mouseY][mouseX] == 0) {
                    xDestinatie = mouseX;
                    yDestinatie = mouseY;
                    mutaPiesa(xDestinatie, yDestinatie);
                    piesaSelectata = false;
                }
            }
        }
    }

    if (piesaSelectata) {
        PiesaAlbaSprite.setPosition(sf::Mouse::getPosition(window).x - DimensiunePatrat / 2, sf::Mouse::getPosition(window).y - DimensiunePatrat / 2);
    }
}

void deseneazaTabla() {
    for (int i = 0; i < NumarLinii; i++) {
        for (int j = 0; j < NumarColoane; j++) {
            sf::RectangleShape patrat;
            patrat.setSize(sf::Vector2f(DimensiunePatrat, DimensiunePatrat));
            patrat.setPosition(i * DimensiunePatrat, j * DimensiunePatrat);

            if ((i + j) % 2 == 1) {
                patrat.setFillColor(sf::Color(119, 153, 84, 255));
            }
            else {
                patrat.setFillColor(sf::Color(233, 237, 204, 255));
            }

            window.draw(patrat);
        }
    }



}

bool verificaCastigator() {
    int numarPieseJucator1 = 0;
    int numarPieseJucator2 = 0;

    // Numără piesele fiecărui jucător
    for (int i = 0; i < NumarLinii; i++) {
        for (int j = 0; j < NumarColoane; j++) {
            if (tabla[j][i] == 1) {
                numarPieseJucator1++;
            }
            else if (tabla[j][i] == 2) {
                numarPieseJucator2++;
            }
        }
    }

    // Verifică dacă vreun jucător a rămas fără piese
    if (numarPieseJucator1 == 0) {
        std::cout << "Jucatorul 2 a castigat!" << std::endl;
        castigator = 2;
        return true;
    }
    else if (numarPieseJucator2 == 0) {
        std::cout << "Jucatorul 1 a castigat!" << std::endl;
        castigator = 1;
        return true;
    }

    if (remainingTimeA <= sf::seconds(0)) {
        std::cout << "Jucatorul 2 a castigat datorita expirarii timpului!" << std::endl;
        castigator = 2;
        return true;
    }
    else if (remainingTimeB <= sf::seconds(0)) {
        std::cout << "Jucatorul 1 a castigat datorita expirarii timpului!" << std::endl;
        castigator = 1;
        return true;
    }

    return false;
}

void coloreazaPatratelePosibile(int piesaSelectataX, int piesaSelectataY) {
    for (int i = 0; i < NumarLinii; i++) {
        for (int j = 0; j < NumarColoane; j++) {
            if (mutareValida(piesaSelectataX, piesaSelectataY, i, j)) {
                sf::RectangleShape patrat;
                patrat.setSize(sf::Vector2f(DimensiunePatrat, DimensiunePatrat));
                patrat.setPosition(i * DimensiunePatrat, j * DimensiunePatrat);
                patrat.setFillColor(sf::Color(255, 0, 0, 150)); // Rosu cu o opacitate redusa

                window.draw(patrat);
            }
        }
    }
}

void deseneazaTitleScreen() {
    sf::Texture titleTexture;
    if (!titleTexture.loadFromFile("Texturi/titlu.png")) {
        throw std::runtime_error("Eroare la încărcarea imaginii pentru title screen!");
    }

    sf::Sprite titleSprite(titleTexture);
    window.draw(titleSprite);
    window.display();

    // Așteaptă până când se apasă Enter
    sf::Event event;
    while (window.waitEvent(event)) {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
            sound1.play();
            window.clear();
            break;
        }
    }
}

void GestionareMeniu()
{
    sf::Texture meniuSMTexture;
    sf::Texture meniuSTexture;
    sf::Texture meniuMTexture;
    sf::Texture meniuTexture;
    if (!meniuMTexture.loadFromFile("Texturi/Meniu.Muzica.png")) {
        throw std::runtime_error("Eroare la încărcarea imaginii pentru title screen!");
    }
    if (!meniuSMTexture.loadFromFile("Texturi/Meniu.sunet+muzica.png")) {
        throw std::runtime_error("Eroare la încărcarea imaginii pentru title screen!");
    }
    if (!meniuSTexture.loadFromFile("Texturi/Meniu.sunet.png")) {
        throw std::runtime_error("Eroare la încărcarea imaginii pentru title screen!");
    }
    if (!meniuTexture.loadFromFile("Texturi/Meniu.surd.png")) {
        throw std::runtime_error("Eroare la încărcarea imaginii pentru title screen!");
    }

    sf::Sprite meniuSMSprite(meniuSMTexture);
    sf::Sprite meniuMSprite(meniuMTexture);
    sf::Sprite meniuSSprite(meniuSTexture);
    sf::Sprite meniuSprite(meniuTexture);
    window.draw(meniuSMSprite);
    window.display();

    // Chenarul pentru modul de joc 1
    sf::RectangleShape chenarMod1Joc(sf::Vector2f(708.0f, 130.0f));
    chenarMod1Joc.setPosition(1172.0f, 318.0f);
    chenarMod1Joc.setFillColor(sf::Color::Transparent);
    chenarMod1Joc.setOutlineThickness(2.0f);
    chenarMod1Joc.setOutlineColor(sf::Color::Transparent);

    // Chenarul pentru modul de joc 2
    sf::RectangleShape chenarMod2Joc(sf::Vector2f(650.0f, 120.0f));
    chenarMod2Joc.setPosition(1200.0f, 510.0f);
    chenarMod2Joc.setFillColor(sf::Color::Transparent);
    chenarMod2Joc.setOutlineThickness(2.0f);
    chenarMod2Joc.setOutlineColor(sf::Color::Transparent);

    // Chenarul pentru modul de joc 3
    sf::RectangleShape chenarMod3Joc(sf::Vector2f(220.0f, 120.0f));
    chenarMod3Joc.setPosition(1400.0f, 678.0f);
    chenarMod3Joc.setFillColor(sf::Color::Transparent);
    chenarMod3Joc.setOutlineThickness(2.0f);
    chenarMod3Joc.setOutlineColor(sf::Color::Transparent);


    sf::RectangleShape chenarMod4Joc(sf::Vector2f(92.0f, 88.0f));
    chenarMod4Joc.setPosition(1680.0f, 966.0f);
    chenarMod4Joc.setFillColor(sf::Color::Transparent);
    chenarMod4Joc.setOutlineThickness(2.0f);
    chenarMod4Joc.setOutlineColor(sf::Color::Transparent);


    sf::RectangleShape chenarMod5Joc(sf::Vector2f(92.0f, 88.0f));
    chenarMod5Joc.setPosition(1805.0f, 966.0f);
    chenarMod5Joc.setFillColor(sf::Color::Transparent);
    chenarMod5Joc.setOutlineThickness(2.0f);
    chenarMod5Joc.setOutlineColor(sf::Color::Transparent);



    sf::Event event;
    while (modJoc == 0) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

                // Verifică dacă s-a apăsat pe chenarul Mod de Joc
                if (chenarMod1Joc.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                    modJoc = 1;  // Sau setează modul de joc dorit
                    if (sunet == 1) {
                        sound1.play();
                        sunetSelectieMod.play();
                    }
                }
                if (chenarMod2Joc.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                    modJoc = 2;  // Sau setează modul de joc dorit
                    if (sunet == 1) {
                        sound1.play();
                        sunetSelectieMod.play();
                    }
                }
                if (chenarMod3Joc.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                    if (sunet == 1)
                        sound1.play();

                    window.close();  // Sau setează modul de joc dorit
                }
                if (chenarMod4Joc.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                    // Se opreste muzica
                    if (sunet == 1)
                        sound1.play();
                    if (muzica == 1) {
                        muzica = 0;
                        music1.pause();
                    }
                    else
                    {
                        muzica = 1;
                        music1.play();
                    }


                }
                if (chenarMod5Joc.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                    if (sunet == 1)
                        sound1.play();
                    if (sunet == 1)
                        sunet = 0;
                    else
                    {
                        sunet = 1;
                    }
                }

            }
        }

        window.clear();
        if (muzica == 1 && sunet == 1)
        {

            window.draw(meniuSMSprite);

        }
        else if (muzica == 0 && sunet == 1)
        {

            window.draw(meniuSSprite);
        }
        else if (muzica == 1 && sunet == 0)
        {

            window.draw(meniuMSprite);
        }
        else if (muzica == 0 && sunet == 0)
        {

            window.draw(meniuSprite);
        }
        window.draw(chenarMod1Joc);
        window.draw(chenarMod2Joc);
        window.draw(chenarMod3Joc);
        window.draw(chenarMod4Joc);
        window.draw(chenarMod5Joc);
        window.display();
    }
}

void resetJoc() {
    for (int i = 0; i < NumarLinii; i++) {
        for (int j = 0; j < NumarColoane; j++) {
            // Repopulează tabla cu piesele inițiale
            if ((i + j) % 2 == 1) {
                tabla[j][i] = (j < 2) ? 1 : ((j > 5) ? 2 : 0);
            }
            else {
                tabla[j][i] = 0;
            }
        }
    }

    // Resetarea altor variabile de stare
    culoareTura = 1;
    TurePace = 8;
    Ture = 0;
    piesaSelectata = false;
    piesaSelectataX = -1;
    piesaSelectataY = -1;
    xDestinatie = -1;
    yDestinatie = -1;

    // Resetează timpul pe ceasurile jucătorilor
    clockA.restart();
    clockB.restart();
    remainingTimeA = sf::seconds(300);
    remainingTimeB = sf::seconds(300);

    startClock();
}

void afiseazaEcranCastigJucator1() {
    sf::Sprite spriteCastigJucator1(TexturaCastigJucator1);
    window.draw(spriteCastigJucator1);



    sf::RectangleShape chenarM(sf::Vector2f(471.0f, 118.0f));
    chenarM.setPosition(725.0f, 410.0f);
    chenarM.setFillColor(sf::Color::Transparent);
    chenarM.setOutlineThickness(2.0f);
    chenarM.setOutlineColor(sf::Color::Transparent);

    sf::RectangleShape chenarQ(sf::Vector2f(215.0f, 118.0f));
    chenarQ.setPosition(840.0f, 560.0f);
    chenarQ.setFillColor(sf::Color::Transparent);
    chenarQ.setOutlineThickness(2.0f);
    chenarQ.setOutlineColor(sf::Color::Transparent);


    window.draw(chenarQ);

    window.draw(chenarM);
    window.display();
    modJoc = 3;
    // Așteaptă un clic în chenar
    while (modJoc == 3) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.clear();
                modJoc = 0; // Setează modul jocului la 0 pentru a reveni la meniu
                return;
            }
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                // Obține poziția locală la chenarM


                // Verifică dacă poziția mouse-ului se află în chenar
                if (chenarM.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {

                    modJoc = 2;
                    resetJoc();

                }
                if (chenarQ.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {

                    window.clear();
                    modJoc = 0;


                }
            }
        }
    }
}

void afiseazaEcranCastigJucator2() {
    sf::Sprite spriteCastigJucator2(TexturaCastigJucator2);
    window.draw(spriteCastigJucator2);

    sf::RectangleShape chenarM(sf::Vector2f(471.0f, 118.0f));
    chenarM.setPosition(725.0f, 410.0f);
    chenarM.setFillColor(sf::Color::Transparent);
    chenarM.setOutlineThickness(2.0f);
    chenarM.setOutlineColor(sf::Color::Transparent);

    sf::RectangleShape chenarQ(sf::Vector2f(215.0f, 118.0f));
    chenarQ.setPosition(840.0f, 560.0f);
    chenarQ.setFillColor(sf::Color::Transparent);
    chenarQ.setOutlineThickness(2.0f);
    chenarQ.setOutlineColor(sf::Color::Transparent);


    window.draw(chenarQ);

    window.draw(chenarM);
    window.display();
    modJoc = 3;
    // Așteaptă un clic în chenar
    while (modJoc == 3) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.clear();
                modJoc = 0; // Setează modul jocului la 0 pentru a reveni la meniu
                return;
            }
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                // Obține poziția locală la chenarM


                // Verifică dacă poziția mouse-ului se află în chenar
                if (chenarM.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {

                    modJoc = 2;
                    resetJoc();

                }
                if (chenarQ.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {

                    window.clear();
                    modJoc = 0;


                }
            }
        }
    }
}

void afiseazaEcranPierdutJucator() {
    sf::Sprite spritePierdutJucator(TexturaPierdutJucator);
    window.draw(spritePierdutJucator);

    sf::RectangleShape chenarM(sf::Vector2f(471.0f, 118.0f));
    chenarM.setPosition(725.0f, 410.0f);
    chenarM.setFillColor(sf::Color::Transparent);
    chenarM.setOutlineThickness(2.0f);
    chenarM.setOutlineColor(sf::Color::Transparent);

    sf::RectangleShape chenarQ(sf::Vector2f(215.0f, 118.0f));
    chenarQ.setPosition(840.0f, 560.0f);
    chenarQ.setFillColor(sf::Color::Transparent);
    chenarQ.setOutlineThickness(2.0f);
    chenarQ.setOutlineColor(sf::Color::Transparent);


    window.draw(chenarQ);
    window.draw(chenarM);

    window.display();
    modJoc = 3;
    // Așteaptă un clic în chenar
    while (modJoc == 3) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.clear();
                modJoc = 0; // Setează modul jocului la 0 pentru a reveni la meniu
                return;
            }
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                // Obține poziția locală la chenarM


                // Verifică dacă poziția mouse-ului se află în chenar
                if (chenarM.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {

                    modJoc = 1;
                    resetJoc();

                }
                if (chenarQ.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {

                    window.clear();
                    modJoc = 0;


                }
            }
        }
    }
}

void afiseazaEcranCastigJucator() {
    sf::Sprite spriteCastigJucator(TexturaCastigJucator);
    window.draw(spriteCastigJucator);



    sf::RectangleShape chenarM(sf::Vector2f(471.0f, 118.0f));
    chenarM.setPosition(725.0f, 410.0f);
    chenarM.setFillColor(sf::Color::Transparent);
    chenarM.setOutlineThickness(2.0f);
    chenarM.setOutlineColor(sf::Color::Transparent);

    sf::RectangleShape chenarQ(sf::Vector2f(215.0f, 118.0f));
    chenarQ.setPosition(840.0f, 560.0f);
    chenarQ.setFillColor(sf::Color::Transparent);
    chenarQ.setOutlineThickness(2.0f);
    chenarQ.setOutlineColor(sf::Color::Transparent);


    window.draw(chenarQ);
    window.draw(chenarM);

    window.display();
    modJoc = 3;
    // Așteaptă un clic în chenar
    while (modJoc == 3) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.clear();
                modJoc = 0; // Setează modul jocului la 0 pentru a reveni la meniu
                return;
            }
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                // Obține poziția locală la chenarM


                // Verifică dacă poziția mouse-ului se află în chenar
                if (chenarM.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {

                    modJoc = 1;
                    resetJoc();

                }
                if (chenarQ.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {

                    window.clear();
                    modJoc = 0;


                }
            }
        }
    }
}

int main() {
    initializeazaJoc();
    music1.play();

    // Desenează title screen
    deseneazaTitleScreen();
    GestionareMeniu();

    clockA.restart();
    Start = 1;

    while (window.isOpen()) {
        // Restul codului rămâne neschimbat

        window.clear();
        if (modJoc == 0) {
            GestionareMeniu(); // Afișează meniul principal
            resetJoc();
        }
        if (modJoc == 2) {
            deseneazaTabla();
            initializeazaUI();
            proceseazaEvenimente();
            if (piesaSelectataX != -1 && piesaSelectataY != -1) {
                coloreazaPatratelePosibile(piesaSelectataX, piesaSelectataY);
            }
            afiseazaPiesele();

            if (verificaCastigator()) {
                if (castigator == 1) {
                    afiseazaEcranCastigJucator1();
                }
                else if (castigator == 2) {
                    afiseazaEcranCastigJucator2();
                }
                resetJoc(); // Opțional, resetează jocul după afiș
                castigator = 0; // Resetăm castigatorul
            }
        }
        else if (modJoc == 1) {
            deseneazaTabla();
            initializeazaUI();
            ProceseazaEvenimenteCalculator();
            if (piesaSelectataX != -1 && piesaSelectataY != -1) {
                coloreazaPatratelePosibile(piesaSelectataX, piesaSelectataY);
            }
            afiseazaPiesele();
            if (verificaCastigator()) {
                if (castigator == 1) {
                    afiseazaEcranCastigJucator();
                }
                else if (castigator == 2) {
                    afiseazaEcranPierdutJucator();
                }
                resetJoc(); // Opțional, resetează jocul după afiș
                castigator = 0; // Resetăm castigatorul
            }
        }
        window.display();
    }
    return 0;
}



