using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.OleDb;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;

namespace BuyMe
{
    public partial class MainWnd : Form
    {
        const int MERCHANT_ID = 123456;
        const string dbConString = @"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=Database11.accdb;";

        const int KEYBOARD_HEIGHT = 150;


        XmlDocument xmlDoc;
        XmlNodeList availablePapers;

        PaperCollection availablePapersCollection = new PaperCollection();

        public MainWnd()
        {
            InitializeComponent();
        }

        private void loadData()
        {
            xmlDoc = new XmlDocument();
            xmlDoc.Load("ziare.xml");

            availablePapers = xmlDoc.SelectNodes("/cifre-difuzare/publicatii/publicatie[cifre/total_vanzari >= 15000]");

            foreach (XmlNode paper in availablePapers)
            {
                PaperStruct paperStruct = new PaperStruct().withNume(paper["nume"].InnerText)
                                                           .withEditor(paper["editor"].InnerText)
                                                           .withCategorie(paper["categorie"].InnerText)
                                                           .withPeriodicitate(paper["periodicitate"].InnerText)
                                                           .withArie(paper["arie"].InnerText)
                                                           .withTirajBrut(int.Parse(paper["cifre"]["tiraj_brut"].InnerText))
                                                           .withTotalVanzari(int.Parse(paper["cifre"]["total_vanzari"].InnerText))
                                                           .withTotalDifuzat(int.Parse(paper["cifre"]["total_difuzat"].InnerText))
                                                           .withSursa(paper["cifre"]["sursa"].InnerText)
                                                           .withPretSimplu(paper["cifre"]["pret_simplu"] == null ? 0 : double.Parse(paper["cifre"]["pret_simplu"].InnerText))
                                                           .withPretAddon(paper["cifre"]["pret_addon"] == null ? 0 : double.Parse(paper["cifre"]["pret_addon"].InnerText));

                availablePapersCollection.add(paperStruct);
                
            }

        }

        PaperCollection checkoutPapersCollection = new PaperCollection();

        private void addPaperToCheckout(PaperStruct paperStruct)
        {
            // copie deep a obiectului
            using (MemoryStream memoryStream = new MemoryStream())
            {
                var binaryFormatter = new BinaryFormatter();
                binaryFormatter.Serialize(memoryStream, paperStruct);
                memoryStream.Position = 0;

                checkoutPapersCollection.add((PaperStruct)binaryFormatter.Deserialize(memoryStream));
            }

            lastPaperNameLabel.Text = paperStruct.nume;

            if (paperStruct.includeAddon)
                lastPaperNameLabel.Text += " (cu CD/carte)";
        }

        private void undoRemovePaperFromCheckout()
        {
            if (checkoutPapersCollection.count()  == 0)
            {
                MessageBox.Show("Nu exista ziare care sa poata fi sterse.");
            }

            checkoutPapersCollection.removeLast();

            PaperStruct paperStruct = checkoutPapersCollection.getLastElement();

            if (paperStruct != null)
            {
                lastPaperNameLabel.Text = paperStruct.nume;

                if (paperStruct.includeAddon)
                    lastPaperNameLabel.Text += " (cu CD/carte)";
            }
            else
                lastPaperNameLabel.Text = "Niciun ziar adaugat";
        }

        Point slideOutLocation;
        Point slideInLocation;

        private void MainWnd_Load(object sender, EventArgs e)
        {
            paperListPanel1.setPaperListPanelActions(addPaperToCheckout, undoRemovePaperFromCheckout);

            totalPricePanel.Location = new Point(paperListPanel1.Location.X - paperListPanel1.Width, paperListPanel1.Location.Y);
            slideOutLocation = new Point(totalPricePanel.Location.X, totalPricePanel.Location.Y);
            slideInLocation = new Point(paperListPanel1.Location.X, paperListPanel1.Location.Y);

            loadData();
            


            paperNameTextbox_TextChanged(null, null);
        }

        private void switchPanels()
        {
            Timer t1 = new Timer();
            t1.Interval = 10;

            if (totalPricePanel.Location.X - 10 < slideOutLocation.X)
            {
                t1.Tick += (sender, e) => T1_Tick(sender, e, true, t1);
                endSaleButton.Text = "Vanzare noua";

                undoRemovePaperButton.Enabled = false;
                paperNameTextbox.Enabled = false;
            }
            else
            {
                t1.Tick += (sender, e) => T1_Tick(sender, e, false, t1);
                endSaleButton.Text = "Incheie vanzarea";


                // pregatesc o alta vanzare
                lastPaperNameLabel.Text = "Niciun ziar adaugat";
                checkoutPapersCollection.clear();

                undoRemovePaperButton.Enabled = true;
                paperNameTextbox.Enabled = true;
            }

            t1.Start();
        }

        private void T1_Tick(object sender, EventArgs e, bool slideIn, Timer t1)
        {
            if (slideIn)
            {
                if (totalPricePanel.Location.X + 10 < slideInLocation.X)
                    totalPricePanel.Location = new Point(totalPricePanel.Location.X + 15, totalPricePanel.Location.Y);
                else
                {
                    totalPricePanel.Location = new Point(slideInLocation.X, slideInLocation.Y);
                    t1.Stop();
                }
            }
            else
            {
                if (totalPricePanel.Location.X - 15 > slideOutLocation.X)
                    totalPricePanel.Location = new Point(totalPricePanel.Location.X - 15, totalPricePanel.Location.Y);
                else
                {
                    totalPricePanel.Location = new Point(slideOutLocation.X, slideOutLocation.Y);
                    t1.Stop();
                }

            }
        }

        private void paperNameTextbox_TextChanged(object sender, EventArgs e)
        {
            

            // cauta un ziar in lista
            paperListPanel1.clear();
            List<PaperStruct> matchedPapers = availablePapersCollection.findPaperByName(paperNameTextbox.Text);

            foreach (PaperStruct matchedPaper in matchedPapers)
            {
                paperListPanel1.add(matchedPaper);
            }
        }

        private void undoRemovePaperButton_Click(object sender, EventArgs e)
        {
            undoRemovePaperFromCheckout();
        }


        private void endSaleButton_Click(object sender, EventArgs e)
        {
            if (checkoutPapersCollection.count() == 0 && endSaleButton.Text.Contains("Incheie vanzarea"))
            {
                MessageBox.Show("Nu exista ziare adaugate pentru vanzare.");
                return;
            }


            

            if (endSaleButton.Text.Contains("Vanzare noua"))
            {
                switchPanels();
                return;
            }

            // tranzitie panel
            switchPanels();

            double totalPrice = checkoutPapersCollection.getTotalPrice();
            totalPriceLabel.Text = totalPrice.ToString() + " lei";

            // adaugare in DB
            OleDbConnection dbConnection = new OleDbConnection(dbConString);
            dbConnection.Open();

            string transactionUid = DateTime.Now.ToString("yyyyMMddHHmmssffff");

            OleDbCommand query = new OleDbCommand(@"INSERT INTO transactions (merchant_id, transaction_id, total)
                                                                      VALUES (" + MERCHANT_ID + ", " + transactionUid + "," + totalPrice.ToString() + ")", dbConnection);
            query.ExecuteScalar();

            foreach (PaperStruct paperStruct in checkoutPapersCollection.findPaperByName(string.Empty))
            {
                string name = paperStruct.nume;
                if (paperStruct.includeAddon)
                    name += " (cu CD/carte)";

                query = new OleDbCommand(@"INSERT INTO paper_sales (transaction_id, paper_name)
                                                             VALUES (" + transactionUid + ",'" + name + "')", dbConnection);

                query.ExecuteScalar();
            }

            dbConnection.Close();

        }

        int keyboardMode = 1;

        private void simulateKeyboardLabel_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            paperListPanel1.Height -= KEYBOARD_HEIGHT * keyboardMode;
            totalPricePanel.Height -= KEYBOARD_HEIGHT * keyboardMode;

            groupBox1.Location = new Point(groupBox1.Location.X, groupBox1.Location.Y - KEYBOARD_HEIGHT * keyboardMode);
            endSaleButton.Location = new Point(endSaleButton.Location.X, endSaleButton.Location.Y - KEYBOARD_HEIGHT * keyboardMode);

            paperNameTextbox.Location = new Point(paperNameTextbox.Location.X, paperNameTextbox.Location.Y - KEYBOARD_HEIGHT * keyboardMode);

            keyboardMode *= -1;
        }
    }
}
