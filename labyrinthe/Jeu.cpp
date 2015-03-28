#include "Jeu.h"
#include "Commande.h"
#include "Menu.h"
#include "Espace.h"

#include<iostream>
using std::ostream;

const CPosition
CJeu::POS_PERSO_DEFAUT = CPosition(1, 1),
CJeu::POS_ITEMS_DEFAUT = CPosition(5, 5),
CJeu::POS_OBJECTIF_DEFAUT = CPosition(10, 10);

const char CJeu::ESPACE_VIDE = ' ';

bool CJeu::Gagne()
{
	return perso_.GetPosition() == objectif_;
}

bool CJeu::Perd()
{
	return perso_.EstMort();
}

bool CJeu::Fini()
{
	return Perd() || Gagne();
}

void CJeu::AfficherEtat(ostream & os) const
{
	system("cls");
	for (int i = CEspace::Y_MIN; i < CEspace::Y_MAX; ++i)
	{
		for (int j = CEspace::X_MIN; j < CEspace::X_MAX; ++j)
		{
			const CPosition pos(j, i);
			if (pos == perso_.GetPosition())
			{
				os << perso_;
			}
			else
				os << ESPACE_VIDE;
		}
	}
}

void CJeu::Executer(const CCommande & c)
{
	if (c == CMenu::AVANCER)
	{
		perso_.Avancer();
	}
	else if (c == CMenu::DROITE)
	{
		perso_.Droite();
	}
	else if (c == CMenu::GAUCHE)
	{
		perso_.Gauche();
	}
	else if (c == CMenu::RECULER)
	{
		perso_.Reculer();
	}
}