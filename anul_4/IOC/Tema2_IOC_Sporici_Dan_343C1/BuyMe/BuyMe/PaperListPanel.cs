using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BuyMe
{
    class PaperListPanel : Panel
    {
        private List<Panel> paperListElems;

        Action<PaperStruct> addPaperToCheckout;
        Action removePaperFromCheckout;

        [DllImport("user32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool ShowScrollBar(IntPtr hWnd, int wBar, bool bShow);

        public PaperListPanel()
        {
            paperListElems = new List<Panel>();

            this.AutoScroll = true;
            this.BackColor = Color.White;

            
        }

        

        

        public void setPaperListPanelActions(Action<PaperStruct> addPaperToCheckout, Action removePaperFromCheckout)
        {
            this.addPaperToCheckout = addPaperToCheckout;
            this.removePaperFromCheckout = removePaperFromCheckout;
        }

        public void add(PaperStruct paperStruct)
        {
            // vreau ca acel scrollbar sa apara mereu acolo (altfel se ingusteaza panel-ul cand sunt prea putine elemente)
            ShowScrollBar(this.Handle, 1, true);

            Panel p = new Panel();
            p.Width = this.Width - 20;
            p.Height = 60;
            p.Location = new Point(0, paperListElems.Count * (p.Height + 10));

            p.BackColor = Color.FromArgb(219, 213, 110);

            // -- label (titlu)
            Label titleLabel = new Label();

            titleLabel.Text = paperStruct.nume;
            titleLabel.Location = new Point(10, 10);
            titleLabel.Font = new Font("Calibri", 12);
            titleLabel.AutoSize = true;
            titleLabel.MaximumSize = new Size(p.Width - titleLabel.Location.X - 60, 0);

            titleLabel.Click += (sender, e) => P_Click(sender, e, paperStruct, p);
            p.Click += (sender, e) => P_Click(sender, e, paperStruct, p);
            p.Controls.Add(titleLabel);


            // -- optiuni
            Panel p2 = new Panel();
            p2.Width = (int)(p.Width / 3.4);
            p2.Height = (int)(p.Height / 2.5);
            p2.Location = new Point(p.Width - 60, 10);
            p2.BorderStyle = BorderStyle.FixedSingle;

            CheckBox ch1 = new CheckBox();
            ch1.Text = "cu CD / carte";
            ch1.AutoSize = false;
            ch1.Font = new Font("Calibri", 8);
            ch1.MaximumSize = new Size(70, 100);
            ch1.MinimumSize = new Size(30, 70);
            ch1.Click += (sender, e) => Ch1_Click(sender, e, paperStruct);
            
            ch1.Location = new Point(p2.Width / 2 - ch1.Width / 2 + 10, p2.Height / 2 - ch1.Height/2);

            

            p2.Controls.Add(ch1);

            p.Controls.Add(p2);
            

            Controls.Add(p);
            paperListElems.Add(p);
        }

        private void Ch1_Click(object sender, EventArgs e, PaperStruct paperStruct)
        {

            if (((CheckBox)sender).Checked)
                paperStruct.includeAddon = true;
            else
                paperStruct.includeAddon = false;

            
        }

        private void P_Click(object sender, EventArgs e, PaperStruct paperStruct, Panel sourcePanel)
        {
            addPaperToCheckout(paperStruct);

            // mica animatie pt click pe adaugare ziar
            Timer t1 = new Timer();

            sourcePanel.BackColor = Color.FromArgb(249, 243, 140);

            t1.Interval = 100;
            t1.Tick += (sender2, e2) => T1_Tick(sender2, e2, sourcePanel, t1);
            t1.Start();
           
        }

        private void T1_Tick(object sender, EventArgs e, Panel sourcePanel, Timer t1)
        {
            sourcePanel.BackColor = Color.FromArgb(sourcePanel.BackColor.R + (219 - sourcePanel.BackColor.R) / 5,
                                                   sourcePanel.BackColor.G + (213 - sourcePanel.BackColor.G) / 5,
                                                   sourcePanel.BackColor.B + (110 - sourcePanel.BackColor.B) / 5);

            if (Math.Abs(sourcePanel.BackColor.R - 219) < 6 &&
                Math.Abs(sourcePanel.BackColor.G - 213) < 6 &&
                Math.Abs(sourcePanel.BackColor.B - 110) < 6)
            {
                sourcePanel.BackColor = Color.FromArgb(219, 213, 110);
                t1.Stop();
            }

        }

        public void clear()
        {
            foreach (Panel paperListElem in paperListElems)
                Controls.Remove(paperListElem);

            paperListElems.Clear();
        }

       
    }
}
