#pragma once;
#include "Position.h";
#include "Orientation.h";

class CPersonnage
{
public:
	CPersonnage();

	CPersonnage(CPosition pos, unsigned short pasDepart = NB_PAS_DEFAUT, 
				unsigned short visionDepart = VISION_DEFAUT, unsigned short vitesse = VITESSE_DEFAUT);

	CPersonnage(unsigned short x, unsigned short y, unsigned short pasDepart = NB_PAS_DEFAUT,
				unsigned short visionDepart = VISION_DEFAUT, unsigned short vitesse = VITESSE_DEFAUT);

	void SetNbPas(unsigned short pas);
	short GetNbPas();

	void SetVision(unsigned short vision);
	short GetVision();

	void SetVitesse(const unsigned short vitesse);
	unsigned short GetVitesse() const;

	Orientation GetDirection() const;
	void SetDirection(const Orientation);
	CPosition GetPosition() const;

	void Avancer();
	void Reculer();
	void Gauche();
	void Droite();
	bool EstMort();

private:

	static const unsigned short NB_PAS_DEFAUT = 50;
	static const unsigned short VISION_DEFAUT = 3;
	static const unsigned short VITESSE_DEFAUT = 2;

	unsigned short nb_pas_;
	unsigned short vision_;
	unsigned short vitesse_;

	CPosition pos_personnage_;
	Orientation direction_;

	void Marcher(const short vitesse);
	void DescendrePas();
};
#include <iosfwd>
using std::ostream;
ostream& operator<<(ostream &, const CPersonnage &);