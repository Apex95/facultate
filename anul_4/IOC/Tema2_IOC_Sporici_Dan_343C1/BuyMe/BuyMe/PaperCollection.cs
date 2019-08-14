using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BuyMe
{
    class PaperCollection
    {
        private List<PaperStruct> papersList;

        public PaperCollection()
        {
            papersList = new List<PaperStruct>();
        }

        public void add(PaperStruct paperStruct)
        {
            papersList.Add(paperStruct);
        }

        public void removeLast()
        {
            if (papersList.Count > 0)
                papersList.RemoveAt(papersList.Count - 1);
        }

        public void remove(PaperStruct paperStruct)
        {
            papersList.Remove(paperStruct);
        }

        public PaperStruct getLastElement()
        {
            if (papersList.Count > 0)
                return papersList.Last();

            return null;
        }


        public List<PaperStruct> findPaperByName(string name)
        {
            return papersList.FindAll(paper => paper.nume.ToLower().Contains(name.ToLower()));
        }

        public double getTotalPrice()
        {
            double totalPrice = 0;

            foreach (PaperStruct paperStruct in papersList)
                totalPrice += paperStruct.includeAddon ? paperStruct.cifre.pret_addon : paperStruct.cifre.pret_simplu;

            return totalPrice;
        }

        public int count()
        {
            return papersList.Count;
        }

        public void clear()
        {
            papersList.Clear();
        }
    }
}
