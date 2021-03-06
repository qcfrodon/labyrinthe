#include "Jeu.h"
#include "Commande.h"
#include "Menu.h"
#include "Espace.h"

#include<iostream>
using std::ostream;
using std::move;

const CPosition
CJeu::POS_PERSO_DEFAUT = CPosition(1, 1),
CJeu::POS_ITEMS_DEFAUT = CPosition(5, 5),
CJeu::POS_OBJECTIF_DEFAUT = CPosition(10, 10);

const char CJeu::VIDE = ' ';
const string CJeu::ESPACE_JEU_DEFAUT = "Map.txt";

CJeu::CJeu() throw() : perso_(POS_PERSO_DEFAUT), 
					   espace_jeu_(ESPACE_JEU_DEFAUT)
{
	CEspace::CreerEspace(espace_jeu_);
	PlacerItems(NBTORCH_DEFAUT,NBLIFE_DEFAUT);
	perso_.SetPosition(CEspace::EtablirPosition(CEspace::DEPART));
	CEspace::EtablirVisibles(perso_.GetPosition(), perso_.GetVision());
	objectif_ = CEspace::EtablirPosition(CEspace::FIN);
}

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
	partie_gagne_ = Gagne();
	return partie_gagne_ || Perd();
}

void CJeu::AfficherEtat(ostream & os) const
{
	system("cls");
	for (int i = CEspace::Y_MIN; i < CEspace::Max_Y(); ++i)
	{
		for (int j = CEspace::X_MIN; j < CEspace::Max_X(); ++j)
		{
			const CPosition pos(j, i);

			if (pos == perso_.GetPosition())
				os << perso_;
			else if (CEspace::EstVisible(pos))		
				CEspace::Afficher(os, pos);
			else
				os << VIDE;
		}
		os << '\n';
	}
	os << "Pas restants: " << perso_.GetNbPas() << 
		  " Vision actuelle: " << perso_.GetVision() << '\n';
}

void CJeu::AfficherResultats(ostream & os) const
{
	string msg = PartieGagne() ? "Vous avez gagne!" : "Vous avez perdu...";
	os << msg << '\n';
}

void CJeu::Executer(const CCommande & c)
{
	if (c == CMenu::AVANCER)
	{
		if (DirectionAccessible(perso_.GetDirection()))
		{
			perso_.Avancer();
			DiminuerEffetsItem();
			RamasserItem();
			CEspace::EtablirVisibles(perso_.GetPosition(), perso_.GetVision());
		}
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
		if (DirectionAccessible(perso_.GetDirectionInverse()))
		{
			perso_.Reculer();
			DiminuerEffetsItem();
			RamasserItem();
			CEspace::EtablirVisibles(perso_.GetPosition(), perso_.GetVision());
		}
	}
}

bool CJeu::DirectionAccessible(Orientation direction)
{
	bool marchable = false;
	switch (direction)
	{
	case Nord:
		marchable = CEspace::EstAccessible(perso_.GetPosition().VoisineNord());
		break;
	case Sud:
		marchable = CEspace::EstAccessible(perso_.GetPosition().VoisineSud());
		break;
	case Est:
		marchable = CEspace::EstAccessible(perso_.GetPosition().VoisineEst());
		break;
	case Ouest:
		marchable = CEspace::EstAccessible(perso_.GetPosition().VoisineOuest());
		break;
	}
	return marchable;
}

void CJeu::PlacerTorchesHasard(int nbTorch)
{
	try
	{
		for (int i = 0; i < nbTorch; ++i)
		{
			unique_ptr<CTorcheUpgrade> p(new CTorcheUpgrade(CEspace::TrouverEspaceLibre()));
			items_.push_back(move(p));
		}
	}
	catch (CEspace::MapPleine) 
	{}
}

void CJeu::PlacerViesHasard(int nbLife)
{
	try
	{
		for (int i = 0; i < nbLife; ++i)
		{
			unique_ptr<CLifeUpgrade> p(new CLifeUpgrade(CEspace::TrouverEspaceLibre()));
			items_.push_back(move(p));
		}
	}
	catch (CEspace::MapPleine)
	{}
}

void CJeu::PlacerItems(int nbTorch, int nbLife)
{
	PlacerTorchesHasard(nbTorch);
	PlacerViesHasard(nbLife);
	for (int i = 0; i < items_.size(); ++i)
	{
		CEspace::Placer(items_[i]->GetPosItem(), items_[i]->GetSymbole());
	}
}

int CJeu::EtablirPosItem(CPosition pos)
{
	for (int i = 0; i < items_.size(); ++i)
	{
		if (pos == items_[i]->GetPosItem())
			return i;
	}
	return -1;
}

void CJeu::DiminuerEffetsItem()
{
	for (auto& kv : objets_actifs_) {
		if (--kv.second == 0)
		{
			//ici on appelle ReduireVision �tant donn� que nous n'avons que des torches;
			//il faudra changer un peu cette approche si on ajoute d'autres items ayant une dur�e dans le futur
			perso_.ReduireVision(kv.first->GetBonus()); 
		}
	}
}

void CJeu::RamasserItem()
{
	int posItem = EtablirPosItem(perso_.GetPosition());

	if (posItem >= 0)
	{
		items_[posItem]->UtiliserItem(perso_);
		CEspace::Retirer(items_[posItem]->GetPosItem());
		objets_actifs_[move(items_[posItem])] = items_[posItem]->GetDuree();
		items_.erase(items_.begin() + posItem, items_.begin() + posItem + 1);
	}
}