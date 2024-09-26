#include "bloc.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <cstdlib>
#include <ctime>

#include <iostream>


bloc::bloc(const sf::Texture& TextTruc, sf::RenderWindow &window, float initialX, float initialY): Tiles(TextTruc){
    std::srand(std::time(0));
    InitialiserPOS();
    CouleurAleatoire();
    BlocAleatoire();
    AddrWindow = &window;    
    Tiles.setTextureRect(sf::IntRect(0,0,18,18));
    Tiles.setPosition(initialX, initialY);
    TabX = initialX;
    TabY = initialY;

    int PatterneA[28] = { 
                 0,2,4,6,   //I
                 0,2,4,5,    // L
                 1,3,4,5,   // L reversed      
                 0,2,3,4,   // T 
                 0,2,3,5,    // Z
                 1,2,3,4,     // Z reversed             
                 0,1,2,3     // carré
                    };

    for (int i = 0; i < 28; i++){
        PatterneApp[i] = PatterneA[i];
    }

    for(int i = 0; i <28;  i++) Patterne[i] = PatterneApp[i];

    for(int i = 0; i <9;  i++){
        for(int j = 0; j<24; j++){    // Mise en place de la map
            map[i][j]= 0;
        }
    }
}

bloc::~bloc(){

}

void bloc::DefinitionDeStruct(int Plage, int Pattern, int i, int j){
     if(Plage == 0){
        PosTot.X1 = i;
        PosTot.Y1 = j;
    }else if(Plage == 1){
        PosTot.X2 = i;
        PosTot.Y2 = j;
    }else if(Plage == 2){
        PosTot.X3 = i;
        PosTot.Y3 = j;
    }else if(Plage == 3){
        PosTot.X4 = i;
        PosTot.Y4 = j;
    }
}

void bloc::assembly(){

    const int KpatternInit = NbBloc*4;
    int Kpattern = KpatternInit;
    int tab1D[8],tab2D[4][2];
    for(int i=0;i<8;i++){
        if(Patterne[Kpattern]==i){
            tab1D[i]= CouleurAlea; 
            Kpattern++;
        }else tab1D[i]=0; 
    }
    int k=0, w=0;
    for(int i=0;i<4;i++)for(int j=0;j<2;j++){
        if(tab1D[k] >= 1){
            DefinitionDeStruct(w,KpatternInit,j,i);
            w++;
        }
        map[j][i]=tab1D[k++];
    }
}

bool bloc::checkLine(){

    std::cout << "Checklined : \n";
    for(int i = 0; i<24; i++){
        int compteur = 0;
        for(int j = 0; j<9; j++){
            if(map[j][i] > 0) compteur++;
            if(map[j][i] == 0) break;
            if(compteur == 9) {
                LigneComplete = i;
                return true;
            }
        }
    }      

    return false;
}

void bloc::mouvement(std::string NomMouv){   
    if(NomMouv == "left") DeplacementGauche();
    if(NomMouv == "right") DeplacementDroite();
    if(NomMouv == "down") DeplacementBas();
}

void bloc::DrawTiles(){
    AddrWindow->draw(Tiles);
}

void bloc::drawASprite(sf::Sprite &Tile){
    AddrWindow->draw(Tile);
}

void bloc::DessinerLeTableau(){
    for(int i = 0; i<9; i++){ 
        for(int j = 0; j<24; j++){
            if (map[i][j] >= 1) {

                int Couleur = map[i][j];

                Tiles.setTextureRect(sf::IntRect(18*Couleur,0,18,18));
                Tiles.setPosition(sf::Vector2f(TabX+18*i, TabY+18*j));
                DrawTiles();
            }
        }
    }
    
}

void bloc::Ajouter(int X, int Y, int  Nbr){
    map[X][Y] = Nbr;
}

void bloc::DeplacementBas(){
    if(
        ( (PosTot.Y1 >= 0 && PosTot.Y1 < 23) && (PosTot.Y2 >= 0 && PosTot.Y2 < 23) && (PosTot.Y3 >= 0 && PosTot.Y3 < 23) && (PosTot.Y4 >= 0 && PosTot.Y4 < 23))
        &&
        (      ((map[PosTot.X1][PosTot.Y1+1] == 0) || PosTot.Y1+1 == PosTot.Y2 || PosTot.Y1+1 == PosTot.Y3 || PosTot.Y1+1 == PosTot.Y4)
            && ((map[PosTot.X2][PosTot.Y2+1] == 0) || PosTot.Y2+1 == PosTot.Y3 || PosTot.Y2+1 == PosTot.Y4) 
            && ((map[PosTot.X3][PosTot.Y3+1] == 0) || (PosTot.Y3+1 == PosTot.Y4 && PosTot.X3-1 == PosTot.X4) || (PosTot.Y3+1 == PosTot.Y4 && PosTot.X3 == PosTot.X4))
            && ((map[PosTot.X4][PosTot.Y4+1] == 0) || PosTot.Y4+1 == PosTot.Y1)      
        )
    ){
        Ajouter(PosTot.X1,PosTot.Y1, 0);
        Ajouter(PosTot.X2,PosTot.Y2, 0);
        Ajouter(PosTot.X3,PosTot.Y3, 0);
        Ajouter(PosTot.X4,PosTot.Y4, 0);

        Ajouter(PosTot.X1,PosTot.Y1+1, CouleurAlea);
        Ajouter(PosTot.X2,PosTot.Y2+1, CouleurAlea);
        Ajouter(PosTot.X3,PosTot.Y3+1, CouleurAlea);
        Ajouter(PosTot.X4,PosTot.Y4+1, CouleurAlea);

        PosTot.Y1++;PosTot.Y2++;PosTot.Y3++;PosTot.Y4++;
    }
}

void bloc::DeplacementDroite(){
    if(
        (PosTot.X1 < 8 && PosTot.X2 < 8 && PosTot.X3 < 8 && PosTot.X4 <8)
        &&
        (      ((map[PosTot.X1+1][PosTot.Y1] == 0) || PosTot.X1+1 == PosTot.X2)
            && ((map[PosTot.X2+1][PosTot.Y2] == 0) || PosTot.X2+1 == PosTot.X3) 
            && ((map[PosTot.X3+1][PosTot.Y3] == 0) || PosTot.X3+1 == PosTot.X4)
            && ((map[PosTot.X4+1][PosTot.Y4] == 0) )
        )   
    )
    {

        Ajouter(PosTot.X1,PosTot.Y1, 0);
        Ajouter(PosTot.X2,PosTot.Y2, 0);
        Ajouter(PosTot.X3,PosTot.Y3, 0);
        Ajouter(PosTot.X4,PosTot.Y4, 0);

        Ajouter((PosTot.X1+1),PosTot.Y1, CouleurAlea);
        Ajouter((PosTot.X2+1),PosTot.Y2, CouleurAlea);
        Ajouter((PosTot.X3+1),PosTot.Y3, CouleurAlea);
        Ajouter((PosTot.X4+1),PosTot.Y4, CouleurAlea);

        PosTot.X1++; PosTot.X2++; PosTot.X3++; PosTot.X4++; 
    }
}

void bloc::DeplacementGauche(){
    if(        
        (PosTot.X1 >= 1 && PosTot.X2 >= 1 && PosTot.X3 >= 1 && PosTot.X4 >= 1)
        &&
        (      ((map[PosTot.X1-1][PosTot.Y1] == 0))
            && ((map[PosTot.X2-1][PosTot.Y2] == 0) || PosTot.X2-1 == PosTot.X1) 
            && ((map[PosTot.X3-1][PosTot.Y3] == 0) || PosTot.X3-1 == PosTot.X2)
            && ((map[PosTot.X4-1][PosTot.Y4] == 0) || PosTot.X4-1 == PosTot.X3)
        )   
    ){
        Ajouter(PosTot.X4,PosTot.Y4, 0);
        Ajouter(PosTot.X3,PosTot.Y3, 0);
        Ajouter(PosTot.X1,PosTot.Y1, 0);
        Ajouter(PosTot.X2,PosTot.Y2, 0);

        Ajouter((PosTot.X1-1),PosTot.Y1, CouleurAlea);
        Ajouter((PosTot.X2-1),PosTot.Y2, CouleurAlea);
        Ajouter((PosTot.X3-1),PosTot.Y3, CouleurAlea);
        Ajouter((PosTot.X4-1),PosTot.Y4, CouleurAlea);

       PosTot.X1--; PosTot.X2--; PosTot.X3--; PosTot.X4--;     
    }
}

void bloc::InitialiserPOS(){    
    if (PosTot.Y1 == 0) VPerdu = true;
    PosTot.X1=0; PosTot.X2=0; PosTot.X3=0; PosTot.X4=0;
    PosTot.Y1=0; PosTot.Y2=0; PosTot.Y3=0; PosTot.Y4=0;
}

void bloc::RegenererBloc(){
    CouleurAleatoire();
    BlocAleatoire();
    assembly();
}

void bloc::CouleurAleatoire(){
    CouleurAlea = rand() %6 + 1;
}

void bloc::BlocAleatoire(){
    NbBloc = rand() % 7;
}

bool bloc::DetectionBlocEnBas(){
    if(PosTot.Y4 == 23 || PosTot.Y1 == 23 || PosTot.Y2 == 23 || PosTot.Y3 == 23) return true;
    
    return false;
}

bool bloc::DetectionBlocEmpile(){
    if(map[PosTot.X1][PosTot.Y1+1] >= 1 && !( ((PosTot.X2 == PosTot.X1) && (PosTot.Y2 == PosTot.Y1+1)) || ((PosTot.X1 == PosTot.X3) && (PosTot.Y1+1 == PosTot.Y3)) || ((PosTot.X1 == PosTot.X4) && (PosTot.Y1+1 == PosTot.Y4)) ) ) return true;
    if(map[PosTot.X2][PosTot.Y2+1] >= 1 && !( ((PosTot.X2 == PosTot.X1) && (PosTot.Y2+1 == PosTot.Y1)) || ((PosTot.X2 == PosTot.X3) && (PosTot.Y2+1 == PosTot.Y3)) || ((PosTot.X2 == PosTot.X4) && (PosTot.Y2+1 == PosTot.Y4)) ) ) return true;
    if(map[PosTot.X3][PosTot.Y3+1] >= 1 && !( ((PosTot.X3 == PosTot.X1) && (PosTot.Y3+1 == PosTot.Y1)) || ((PosTot.X2 == PosTot.X3) && (PosTot.Y2 == PosTot.Y3+1)) || ((PosTot.X3 == PosTot.X4) && (PosTot.Y3+1 == PosTot.Y4)) ) ) return true;
    if(map[PosTot.X4][PosTot.Y4+1] >= 1 && !( ((PosTot.X4 == PosTot.X1) && (PosTot.Y4+1 == PosTot.Y1)) || ((PosTot.X4 == PosTot.X3) && (PosTot.Y4+1 == PosTot.Y3)) || ((PosTot.X2 == PosTot.X4) && (PosTot.Y2 == PosTot.Y4+1)) ) ) return true;
    return false;
}

void bloc::ResetBloc(){
    const int* X[] = {&PosTot.X1, &PosTot.X2, &PosTot.X3, &PosTot.X4};
    const int* Y[] = {&PosTot.Y1, &PosTot.Y2, &PosTot.Y3, &PosTot.Y4};
    
    for(int i = 0; i < 4 ; i++){
        if(*Y[i] <= 4 ){
            VPerdu = true;
            break;
        }
    }

    rotation =0;
    InitialiserPOS();
    RegenererBloc();
}

void bloc::VoirLeTableau(){
    std::cout <<"\n\n";
    for(int i =0; i <24 ; i++){
        for(int j=0 ; j<9; j++){
            std::cout << map[j][i];
        }       
        std::cout << "\n";
    }
}

void bloc::ViderTableau(){
    for(int i=0; i<24; i++){
        for(int j=0; i<9; j++) map[i][j] = 0;
    }
}

bool bloc::Perdu(){
    return VPerdu;
}

void bloc::SuppLine(){

    LigneDetruite++;
    int LigneComplete = 0;
    int mapAnnexe[9][24];

    for (int i = 0; i < 24; i++){
        for (int j = 0; j < 9; j++) mapAnnexe[j][i] = map[j][i];  
    }

    for(int i = 0; i<24; i++){
        int compteur = 0;
        for(int j = 0; j<9; j++){
            if(map[j][i] > 0) compteur++;
            if(map[j][i] == 0) break;
            if(compteur == 9){
                LigneComplete = i;
            }
        }
    } 


    for(int i = LigneComplete; i>0; i--){
        for (int j= 1; j < 9; j++){
            map[j][i] = mapAnnexe[j][i-1];  
        }       
    }
}

void bloc::ScoreAdd(std::string TypePts, int Nbr){
    if(TypePts == "Ligne"){
        int PtsBase = Nbr *100;
        int ScoreTmp = PtsBase + (100*(Nbr));
        score += ScoreTmp;
    }

    else if(TypePts == "DescenteRapide"){
        score += Niveau*1;
    }

    else if(TypePts == "DescenteNow"){
        score += Niveau*2;
    }
}

void bloc::ChangementNiveau(){
    if((Niveau+1)*10 == LigneDetruite) Niveau++;
}

void bloc::RotationBloc(){

    

    if(NbBloc == 6) return;
    if((NbBloc == 0) && (PosTot.X1 >= 6)) return;
    if((NbBloc == 0) && (rotation == 1) && (PosTot.Y4 >= 19)) return;

    if((NbBloc !=1 ) && (PosTot.X1 >= 7))return;
    if((NbBloc !=1 ) && (rotation == 1) && (PosTot.Y4 >= 20))return;

    std::cout << "Rotation " << rotation;

    

    int tab16Cases[4][4], tab16CasesR[4][4], tab2D[2][4];
    const int* X[] = {&PosTot.X1, &PosTot.X2, &PosTot.X3, &PosTot.X4};
    const int* Y[] = {&PosTot.Y1, &PosTot.Y2, &PosTot.Y3, &PosTot.Y4};
    int XPlusPtit= *X[0], YPlusPtit= *Y[0];

    for (int i = 0; i < 4; i++){
        if(*X[i]<XPlusPtit) XPlusPtit = *X[i];
    }
    for (int i = 0; i < 4; i++){
        if(*Y[i]<YPlusPtit) YPlusPtit = *Y[i];
    }

    //Si bloc dans la rangée alors on ne fait pas

    if(NbBloc == 0){
        if(rotation == 0){
            for (int i = YPlusPtit; i < YPlusPtit+2; i++){
                for (int j = XPlusPtit+2; j < XPlusPtit+4; j++){
                    if(map[j][i] >0 ) return;
                }   
            }
        }
        if(rotation == 1){
            for (int i = YPlusPtit+2; i < YPlusPtit+4; i++){
                for (int j = XPlusPtit; j < XPlusPtit+2; j++){
                    if(map[j][i] >0 ) return;
                }   
            }
        }
    }
    else{
        if(rotation == 0){
            for (int i = YPlusPtit; i < YPlusPtit+2; i++){
                for (int j = XPlusPtit+2; j < XPlusPtit+3; j++){
                    if(map[j][i] >0 ) return;
                }   
            }
        }
        if(rotation == 1){
            for (int i = YPlusPtit+2; i < YPlusPtit+3; i++){
                for (int j = XPlusPtit; j < XPlusPtit+2; j++){
                    if(map[j][i] >0 ) return;
                }   
            }
        }
    }



    //Initialiser le tableau Pour rotate
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            tab16Cases[j][i] = 0;
        }
    }

    //On initialise tab16case avec map

    if(NbBloc == 0){
        if(rotation == 0){
            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 2; j++){
                tab16Cases[j][i] = map[XPlusPtit+j][YPlusPtit+i];
                }
            }
        }
        else if(rotation == 1){
            for(int i = 0; i < 2; i++){
                for(int j = 0; j < 4; j++){
                tab16Cases[j][i] = map[XPlusPtit+j][YPlusPtit+i];
                }
            }
        }
    }
    else{
        if(rotation == 0){
            for(int i = 0; i < 3; i++){
                for(int j = 0; j < 2; j++){
                tab16Cases[j][i] = map[XPlusPtit+j][YPlusPtit+i];
                }
            }
        }
        else if(rotation == 1){
            for(int i = 0; i < 2; i++){
                for(int j = 0; j < 3; j++){
                tab16Cases[j][i] = map[XPlusPtit+j][YPlusPtit+i];
                }
            }
        }
    }


    //Rotation

    for(int i=0;i<4;i++)for(int j=0;j<4;j++) tab16CasesR[j][3-i]=tab16Cases[i][j];
    for(int i=0;i<4;i++)for(int j=0;j<4;j++) tab16Cases[j][i]=tab16CasesR[j][i];
    


    while(CheckLineLineRotateH(tab16Cases)) SuppLineRotateH(tab16Cases);
    while(CheckLineLineRotateV(tab16Cases)) SuppLineRotateV(tab16Cases);


    // Efface le contenu de map 

    if(NbBloc == 0){
        for (int i = YPlusPtit; i < YPlusPtit+4; i++){
            for(int j = XPlusPtit; j < XPlusPtit+2; j++){
                map[j][i] = 0;
            }
        }
        
        for (int i = YPlusPtit; i < YPlusPtit+2; i++){
            for(int j = XPlusPtit; j < XPlusPtit+4; j++){
                map[j][i] = 0;
            }
        }
    }
    else{
        for (int i = YPlusPtit; i < YPlusPtit+3; i++){
            for(int j = XPlusPtit; j < XPlusPtit+2; j++){
                map[j][i] = 0;
            }
        }
        
        for (int i = YPlusPtit; i < YPlusPtit+2; i++){
            for(int j = XPlusPtit; j < XPlusPtit+3; j++){
                map[j][i] = 0;
            }
        }
    }

    // Tab dans map

    if(NbBloc == 0){
        for (int i = YPlusPtit; i < YPlusPtit+4; i++){
            for(int j = XPlusPtit; j < XPlusPtit+2; j++){
                map[j][i] = tab16Cases[j-XPlusPtit][i-YPlusPtit];
            }
        }

        for (int i = YPlusPtit; i < YPlusPtit+2; i++){
            for(int j = XPlusPtit; j < XPlusPtit+4; j++){
                map[j][i] = tab16Cases[j-XPlusPtit][i-YPlusPtit];
            }
        }
    }
    else{
        for (int i = YPlusPtit; i < YPlusPtit+3; i++){
            for(int j = XPlusPtit; j < XPlusPtit+2; j++){
                map[j][i] = tab16Cases[j-XPlusPtit][i-YPlusPtit];
            }
        }

        for (int i = YPlusPtit; i < YPlusPtit+2; i++){
            for(int j = XPlusPtit; j < XPlusPtit+3; j++){
                map[j][i] = tab16Cases[j-XPlusPtit][i-YPlusPtit];
            }
        }  
    }
    
    
    int k = 0;
    std::cout << "Le Plus petit" << XPlusPtit << YPlusPtit << "\n";
    

    std::cout << "Les Y Puis X : " << PosTot.Y1 << PosTot.Y2 << PosTot.Y3 << PosTot.Y4 << "  "<<  PosTot.X1 << PosTot.X2 << PosTot.X3 << PosTot.X4 << "\n";


    //Initiation des poses en fonction du bloc (a optimiser si pas flemme):

    if(NbBloc == 0){
        if(rotation == 0){
            for (int i = YPlusPtit; i < YPlusPtit+2; i++){
                for(int j = XPlusPtit; j < XPlusPtit+4; j++){
                    if(map[j][i] > 0){
                        if(k == 0){
                            PosTot.X1 = j;
                            PosTot.Y1 = i;
                            k++;
                        }
                        else if(k == 1){
                            PosTot.X2 = j;
                            PosTot.Y2 = i;    
                            k++;    

                        }
                        else if(k == 2){
                            PosTot.X3 = j;
                            PosTot.Y3 = i;
                            k++;                 
                        }
                        else if(k == 3){              
                            PosTot.X4 = j;
                            PosTot.Y4 = i;
                            k++;
                        }
                        else if(k == 4){
                            break;
                        } 
                    } 
                }
                if(k >=4) break;
            }
        }
        else if(rotation == 1){
            for (int i = YPlusPtit; i < YPlusPtit+4; i++){
                for(int j = XPlusPtit; j < XPlusPtit+2; j++){
                    if(map[j][i] > 0){
                        if(k == 0){
                            PosTot.X1 = j;
                            PosTot.Y1 = i;
                            k++;
                        }
                        else if(k == 1){
                            PosTot.X2 = j;
                            PosTot.Y2 = i;    
                            k++;    

                        }
                        else if(k == 2){
                            PosTot.X3 = j;
                            PosTot.Y3 = i;
                            k++;                 
                        }
                        else if(k == 3){              
                            PosTot.X4 = j;
                            PosTot.Y4 = i;
                            k++;
                        }
                        else if(k == 4){
                            break;
                        } 
                    } 
                }
                if(k >=4) break;
            }
            
        }
    }
    else{
        if(rotation == 0){
            for (int i = YPlusPtit; i < YPlusPtit+2; i++){
                for(int j = XPlusPtit; j < XPlusPtit+3; j++){
                    if(map[j][i] > 0){
                        if(k == 0){
                            PosTot.X1 = j;
                            PosTot.Y1 = i;
                            k++;
                        }
                        else if(k == 1){
                            PosTot.X2 = j;
                            PosTot.Y2 = i;    
                            k++;    

                        }
                        else if(k == 2){
                            PosTot.X3 = j;
                            PosTot.Y3 = i;
                            k++;                 
                        }
                        else if(k == 3){              
                            PosTot.X4 = j;
                            PosTot.Y4 = i;
                            k++;
                        }
                        else if(k == 4){
                            break;
                        } 
                    } 
                }
                if(k >=4) break;
            }

        }
        else if(rotation == 1){
            for (int i = YPlusPtit; i < YPlusPtit+3; i++){
                for(int j = XPlusPtit; j < XPlusPtit+2; j++){
                    if(map[j][i] > 0){
                        if(k == 0){
                            PosTot.X1 = j;
                            PosTot.Y1 = i;
                            k++;
                        }
                        else if(k == 1){
                            PosTot.X2 = j;
                            PosTot.Y2 = i;    
                            k++;    

                        }
                        else if(k == 2){
                            PosTot.X3 = j;
                            PosTot.Y3 = i;
                            k++;                 
                        }
                        else if(k == 3){              
                            PosTot.X4 = j;
                            PosTot.Y4 = i;
                            k++;
                        }
                        else if(k == 4){
                            break;
                        } 
                    } 
                }
                if(k >=4) break;
            }
        }

    }

    
    for(int i = YPlusPtit; i < YPlusPtit+4; i++){
        for (int j = XPlusPtit; j < XPlusPtit+4; j++){
            std::cout << map[j][i];
        }
        std::cout << "\n";
        
    }

    rotation++;
    rotation %= 2;
    std::cout << "Les Y Puis X : " << PosTot.Y1 << PosTot.Y2 << PosTot.Y3 << PosTot.Y4 << "  "<<  PosTot.X1 << PosTot.X2 << PosTot.X3 << PosTot.X4 << "\n";
}

bool bloc::CheckLineLineRotateH(int (&Tab)[4][4]){

    std::cout << "CheckLine H \n";
    int Count = 0;
    bool Finito = true;

    for(int i=0;i<4;i++){
        Count = 0;
        for(int j=0;j<4;j++){ 
            std::cout << Tab[j][i];
            if(i == 0 && (Tab[j][i] >= 1)){
                Finito = true;
                break;
            }
            if(Tab[j][i] == 0) Count++;
            else break;
        }         
        std::cout << Count << "\n";
        if(Count == 4) return true;
        if (Finito) break;
    }
    std::cout << "Faux \n";
    return false;
}

bool bloc::CheckLineLineRotateV(int (&Tab)[4][4]){

    int Count = 0;
    bool Finito = false;

    for(int j=0;j<4;j++){
        Count = 0;
        for(int i=0;i<4;i++){ 
            if(j == 0 && (Tab[i][j] >= 1)){
                Finito = true;
                break;
            }
            else if(Tab[i][j] == 0) Count++;
            else break;
        }
        if (Finito) break;
        if(Count == 4){
            return true;
        }
        

    }
    std::cout << "FauxV \n" ;
    return false;
}

void bloc::SuppLineRotateH(int Tab[4][4]){

    int Tab2[4][4];
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){ 
            Tab2[i][j] = Tab[i][j];
        }
    }

    for(int i=0;i<4;i++){
        for(int j=1;j<4;j++){ 
            Tab[i][j] = 0; 
        } 
    }


    for(int i=0;i<4;i++){
        for(int j=1;j<4;j++){ 
            Tab[i][j-1] = Tab2[i][j]; 
        } 
    }

}

void bloc::SuppLineRotateV(int Tab[4][4]) {

    int Tab2[4][4];
    
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){ 
            Tab2[i][j] = Tab[i][j];
        }
    }
    
    for(int i=0;i<4;i++){
        for(int j=1;j<4;j++){ 
            Tab[i][j] = 0; 
        } 
    }


    for(int i=i;i<4;i++){
        for(int j=0;j<4;j++){ 
            Tab[i-1][j] = Tab2[i][j]; 
        } 
    }

}