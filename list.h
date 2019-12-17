//---------------------------------------------------------------------------
// Auteur : Otofraise
// Date   : 13 / 12 / 2004
// What ? : Liste chainee bidirectionnelle
//---------------------------------------------------------------------------
#ifndef class_ListeH
#define class_ListeH
//---------------------------------------------------------------------------
template <class T> class cl_EltLst
{
public:
  cl_EltLst<T>* Precedent;
  cl_EltLst<T>* Suivant;

  T Data;

  //Constructeur & Destructeur
  cl_EltLst(T Val);
};
//---------------------------------------------------------------------------
template <class T> class cl_Lst
{
private:
  cl_EltLst<T>* Premier;
  cl_EltLst<T>* Dernier;

  int NbElt;

public:
  //Constructeur & Destructeur
  cl_Lst<T>(void);
  ~cl_Lst<T>(void);

  //Ajout
  int push_back(T Val);

  //Insertion
  int insert_after(T Val, int Indice);
  int insert_before(T Val, int Indice);

  //Suppression
  void remove(int Indice);
  void clear(void);

  //Acces
  T& operator[](int Indice);
  T& get(int Indice);
  int size(void) { return NbElt; };
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// FONCTIONS DES ELEMENTS
//---------------------------------------------------------------------------
template <class T> cl_EltLst<T>::cl_EltLst(T Val)
{
  Precedent = NULL;
  Suivant = NULL;
  Data = Val;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// FONCTIONS DE LA LISTE
//---------------------------------------------------------------------------
template <class T> cl_Lst<T>::cl_Lst(void)
{
  Premier = NULL;
  Dernier = NULL;
  NbElt = 0;
}
//---------------------------------------------------------------------------
template <class T> cl_Lst<T>::~cl_Lst(void)
{
  clear();
}
//---------------------------------------------------------------------------
template <class T> int cl_Lst<T>::push_back(T Val)
{
  cl_EltLst<T>* Temp = new cl_EltLst<T>(Val);
  Temp->Precedent = Dernier;
  Temp->Suivant = NULL;

  if (!Premier)
  {
    Premier = Temp;
    Dernier = Temp;
  }
  else if (Dernier)
  {
    Dernier->Suivant = Temp;
    Dernier = Temp;
  }

  ++NbElt;

  return NbElt - 1;
}
//---------------------------------------------------------------------------
template <class T> int cl_Lst<T>::insert_after(T Val, int Indice)
{
  int ValRet;
  if (Indice >= 0 && Indice < NbElt)
  {
    cl_EltLst<T>* Temp = new cl_EltLst<T>(Val);
    cl_EltLst<T>* TempAvant = Premier;

    for (int i = 0; i < Indice; ++i)
    {
      if (TempAvant && TempAvant->Suivant)
        TempAvant = TempAvant->Suivant;
    }

    Temp->Suivant = TempAvant->Suivant;
    Temp->Precedent = TempAvant;

    if (TempAvant->Suivant)
      TempAvant->Suivant->Precedent = Temp;
    else
      Dernier = Temp;

    TempAvant->Suivant = Temp;
    ++NbElt;

    ValRet = Indice + 1;
  }
  else if (NbElt == 0)
    ValRet = push_back(Val);

  return ValRet;
}
//---------------------------------------------------------------------------
template <class T> int cl_Lst<T>::insert_before(T Val, int Indice)
{
  int ValRet;
  if (Indice >= 0 && Indice < NbElt)
  {
    cl_EltLst<T>* Temp = new cl_EltLst<T>(Val);
    cl_EltLst<T>* TempApres = Premier;

    for (int i = 0; i < Indice; ++i)
    {
      if (TempApres && TempApres->Suivant)
        TempApres = TempApres->Suivant;
    }

    Temp->Suivant = TempApres;
    Temp->Precedent = TempApres->Precedent;

    if (TempApres->Precedent)
      TempApres->Precedent->Suivant = Temp;
    else
      Premier = Temp;

    TempApres->Precedent = Temp;
    ++NbElt;

    ValRet = Indice;
  }
  else if (NbElt == 0)
    ValRet = push_back(Val);

  return ValRet;
}
//---------------------------------------------------------------------------
template <class T> void cl_Lst<T>::remove(int Indice)
{
  if (Indice >= 0 && Indice < NbElt)
  {
    cl_EltLst<T>* Temp = Premier;

    for (int i = 0; i < Indice; ++i)
    {
      if (Temp && Temp->Suivant)
        Temp = Temp->Suivant;
    }

    if (Temp->Suivant)
      Temp->Suivant->Precedent = Temp->Precedent;
    else
      Dernier = Temp->Precedent;

    if (Temp->Precedent)
      Temp->Precedent->Suivant = Temp->Suivant;
    else
      Premier = Temp->Suivant;

    delete Temp;
    --NbElt;
  }
}
//---------------------------------------------------------------------------
template <class T> void cl_Lst<T>::clear(void)
{
  cl_EltLst<T>* Temp = Premier;
  cl_EltLst<T>* Temp2 = NULL;

  while (Temp)
  {
    if (Temp)
    {
      Temp2 = Temp;
      Temp = Temp->Suivant;
      delete Temp2;
    }
  }

  Premier = NULL;
  Dernier = NULL;
  NbElt = 0;
}
//---------------------------------------------------------------------------
template <class T> T& cl_Lst<T>::get(int Indice)
{
  cl_EltLst<T>* Temp = Premier;

  for (int i = 0; i < Indice && i < NbElt; ++i)
  {
    if (Temp && Temp->Suivant)
      Temp = Temp->Suivant;
  }

  //if (Temp)
    return Temp->Data;
}

template <class T> T& cl_Lst<T>::operator[](int Indice)
{
  return get(Indice);
}

#endif