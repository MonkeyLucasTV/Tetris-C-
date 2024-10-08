#include <SFML/Graphics.hpp>
#include <string>

class bloc{

    private:
        sf::RenderWindow* AddrWindow;
        sf::Sprite Tiles;
        sf::Vector2f position;

        int score =0, Niveau=0, LigneDetruite=0, Vitesse=0,rotation=0, BlocSaved = 8, CouleurSaved;
        int Patterne[28], map[9][24], PatterneApp[28], TabX, TabY, Rotation, CouleurAlea, CouleurAleaSuivant, LigneComplete, NbBloc, NbBlocSuivant;
        struct LesPos{
            int X1,X2,X3,X4,Y1,Y2,Y3,Y4;
        };
        LesPos PosTot;
        bool VPerdu= false;


    public:
        bloc(const sf::Texture& Textruc ,sf::RenderWindow &window, float initialX, float initialY);
        ~bloc();
        void assembly();
        void next();
        void mouvement(std::string NomMouv);
        bool checkLine();
        void SuppLine();
        std::string Score();
        std::string AfficherNiveau();
        void ScoreAdd(std::string TypePts, int Nbr);
        void DrawTiles();
        void drawASprite(sf::Sprite &Tile);
        void DessinerLeTableau();
        inline sf::Vector2f RevoyerPosition() {return Tiles.getPosition(); };
        void Ajouter(int X, int Y,int Nbr);
        void DeplacementGauche();
        void DeplacementDroite();
        void DeplacementBas();
        void InitialiserPOS();
        void RegenererBloc();
        void CouleurAleatoire();
        void BlocAleatoire();
        bool DetectionBlocEnBas();
        void ResetBloc();
        void VoirLeTableau();
        void ViderTableau();
        void DefinitionDeStruct(int Plage, int Pattern, int i, int j);
        bool Perdu();
        inline int VitesseBloc(){ return ((40*100)-(2*Niveau*100))/6;};
        void ChangementNiveau();
        inline void Effacer(){ AddrWindow->clear();};
        inline void Dessiner(){ AddrWindow->display();};
        bool DetectionBlocEmpile();
        void RotationBloc();
        bool CheckLineLineRotateH(int (&Tab)[4][4]);
        bool CheckLineLineRotateV(int (&Tab)[4][4]);
        void SuppLineRotateH(int Tab[4][4]);
        void SuppLineRotateV(int Tab[4][4]);
        inline int AfficherBlocSuivant(){ return NbBlocSuivant; };
        inline int AfficherBlocSaved(){ return BlocSaved;};
        void ChangerBloc();
        void RemplacerBlocSave();
        void Saved();
};