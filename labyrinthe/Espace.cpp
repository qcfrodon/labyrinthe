#include "Espace.h"
#include <stdlib.h>
#include <time.h> 

vector<vector<char>> CEspace::Map_ = {};
vector<CPosition> CEspace::Visibles_ = {};
vector<CPosition> CEspace::Libre_ = {};

void CEspace::CreerEspace(const string &fileName)
{
	ifstream ifs;
	ifs.open(fileName);
	char c;
	int i = 0;

	Map_.push_back(vector<char>());
	while (EOF != (c = ifs.get()))
	{
		if (c == '\n')
		{
			Map_.push_back(vector<char>());
			++i;
		}
		else
		{
			Map_[i].push_back(c);
			if (c == ' ')
				Libre_.push_back(CPosition(Map_[i].size(), i));
		}
	}
	ifs.close();
}

void CEspace::AjouterVisible(const CPosition pos)
{
	if (EstValide(pos))
		Visibles_.push_back(pos);
}

void CEspace::EtablirVisibles(const CPosition & centre, const short & diam)
{
	Visibles_.clear();
	short rayon = (diam - 1) / 2;
	AjouterVisible(CPosition(centre));
	for (short i = 1; i <= rayon; ++i)
	{
		AjouterVisible(CPosition(centre.GetX() - i, centre.GetY() - i));
		AjouterVisible(CPosition(centre.GetX() - i, centre.GetY() - i));
		AjouterVisible(CPosition(centre.GetX() - i, centre.GetY()));
		AjouterVisible(CPosition(centre.GetX() - i, centre.GetY() + i));
		AjouterVisible(CPosition(centre.GetX(), centre.GetY() - i));
		AjouterVisible(CPosition(centre.GetX(), centre.GetY() + i));
		AjouterVisible(CPosition(centre.GetX() + i, centre.GetY() - i));
		AjouterVisible(CPosition(centre.GetX() - i, centre.GetY()));
		AjouterVisible(CPosition(centre.GetX() + i, centre.GetY() + i));
	}
}

void CEspace::Afficher(ostream &os, const CPosition &pos)
{
	os << Map_[pos.GetY()][pos.GetX()];
}

const CPosition CEspace::PositionDebut()
{
	return CPosition();
}

const CPosition CEspace::PositionFin()
{
	return CPosition();
}

CPosition CEspace::TakeSpace()
{
	srand(time(NULL));

	if (Libre_.size() == 0)
	{
		throw MapPleine();
	}

	int posTable = rand() % Libre_.size();

	CPosition posSpace(Libre_[posTable].GetX(), Libre_[posTable].GetY());

	Libre_.erase(Libre_.begin() + posTable, Libre_.begin() + posTable + 1);

	return posSpace;
}

void CEspace::AddSpace(CPosition pos)
{
	Libre_.push_back(pos);
}

void CEspace::PlaceInMap(CPosition pos, char symbol)
{
	Map_[pos.GetY()][pos.GetX()]= symbol;
}

//CPosition CJeu::TrouverPosDepart()
//{
//	for (short i = 0; i < Map_.size(); ++i)
//	{
//		for (short j = 0; j < Map_[i].size(); ++j)
//		{
//			if (Map_[i][j] == CEspace::DEPART)
//			{
//				return CPosition(i, j);
//			}
//		}
//	}
//	return CPosition(0, 0);
//}