using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BuyMe
{
    [Serializable]
    class PaperStruct
    {
        public string nume { get; private set; }
        public string editor { get; private set; }
        public string categorie { get; private set; }
        public string periodicitate { get; private set; }
        public string tip { get; private set; }
        public string arie { get; private set; }
        public PaperStats cifre { get; private set; }

        public bool includeAddon = false;

        public PaperStruct()
        {
            this.cifre = new PaperStats();
        }

        [Serializable]
        public class PaperStats
        {
            public int tiraj_brut { get; private set; }
            public int total_vanzari { get; private set; }
            public int total_difuzat { get; private set; }
            public string sursa { get; private set; }
            public double pret_simplu { get; private set; }
            public double pret_addon { get; private set; }

            public PaperStats withTirajBrut(int tirajBrut)
            {
                tiraj_brut = tirajBrut;
                return this;
            }

            public PaperStats withTotalVanzari(int totalVanzari)
            {
                total_vanzari = totalVanzari;
                return this;
            }

            public PaperStats withTotalDifuzat(int totalDifuzat)
            {
                total_difuzat = totalDifuzat;
                return this;
            }

            public PaperStats withSursa(string sursa)
            {
                this.sursa = sursa;
                return this;
            }

            public PaperStats withPretSimplu(double pret)
            {
                this.pret_simplu = pret;
                return this;
            }

            public PaperStats withPretAddon(double pret)
            {
                this.pret_addon = pret;
                return this;
            }

        }

        public PaperStruct withNume(string nume)
        {
            this.nume = nume;
            return this;
        }

        public PaperStruct withEditor(string editor)
        {
            this.editor = editor;
            return this;
        }

        public PaperStruct withCategorie(string categorie)
        {
            this.categorie = categorie;
            return this;
        }

        public PaperStruct withPeriodicitate(string periodicitate)
        {
            this.periodicitate = periodicitate;
            return this;
        }

        public PaperStruct withTip(string tip)
        {
            this.tip = tip;
            return this;
        }

        public PaperStruct withArie(string arie)
        {
            this.arie = arie;
            return this;
        }

        public PaperStruct withTirajBrut(int tirajBrut)
        {
            this.cifre = cifre.withTirajBrut(tirajBrut);
            return this;
        }

        public PaperStruct withTotalVanzari(int totalVanzari)
        {
            this.cifre = cifre.withTotalVanzari(totalVanzari);
            return this;
        }

        public PaperStruct withTotalDifuzat(int totalDifuzat)
        {
            this.cifre = cifre.withTotalDifuzat(totalDifuzat);
            return this;
        }

        public PaperStruct withSursa(string sursa)
        {
            this.cifre = cifre.withSursa(sursa);
            return this;
        }

        public PaperStruct withPretSimplu(double pret)
        {
            this.cifre = cifre.withPretSimplu(pret);
            return this;
        }

        public PaperStruct withPretAddon(double pret)
        {
            this.cifre = cifre.withPretAddon(pret);
            return this;
        }
    }
}
