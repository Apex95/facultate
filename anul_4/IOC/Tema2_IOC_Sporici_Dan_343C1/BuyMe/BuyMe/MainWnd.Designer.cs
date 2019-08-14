namespace BuyMe
{
    partial class MainWnd
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.paperNameTextbox = new System.Windows.Forms.TextBox();
            this.undoRemovePaperButton = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.lastPaperNameLabel = new System.Windows.Forms.Label();
            this.endSaleButton = new System.Windows.Forms.Button();
            this.totalPricePanel = new System.Windows.Forms.Panel();
            this.totalPriceLabel = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.simulateKeyboardLabel = new System.Windows.Forms.LinkLabel();
            this.paperListPanel1 = new BuyMe.PaperListPanel();
            this.groupBox1.SuspendLayout();
            this.totalPricePanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // paperNameTextbox
            // 
            this.paperNameTextbox.Font = new System.Drawing.Font("Microsoft Sans Serif", 19.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.paperNameTextbox.Location = new System.Drawing.Point(12, 511);
            this.paperNameTextbox.Name = "paperNameTextbox";
            this.paperNameTextbox.Size = new System.Drawing.Size(296, 45);
            this.paperNameTextbox.TabIndex = 1;
            this.paperNameTextbox.TextChanged += new System.EventHandler(this.paperNameTextbox_TextChanged);
            // 
            // undoRemovePaperButton
            // 
            this.undoRemovePaperButton.BackColor = System.Drawing.Color.IndianRed;
            this.undoRemovePaperButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.undoRemovePaperButton.Font = new System.Drawing.Font("Calibri", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.undoRemovePaperButton.ForeColor = System.Drawing.Color.White;
            this.undoRemovePaperButton.Location = new System.Drawing.Point(6, 89);
            this.undoRemovePaperButton.Name = "undoRemovePaperButton";
            this.undoRemovePaperButton.Size = new System.Drawing.Size(176, 34);
            this.undoRemovePaperButton.TabIndex = 4;
            this.undoRemovePaperButton.Text = "Sterge";
            this.undoRemovePaperButton.UseVisualStyleBackColor = false;
            this.undoRemovePaperButton.Click += new System.EventHandler(this.undoRemovePaperButton_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.BackColor = System.Drawing.Color.White;
            this.groupBox1.Controls.Add(this.lastPaperNameLabel);
            this.groupBox1.Controls.Add(this.undoRemovePaperButton);
            this.groupBox1.Font = new System.Drawing.Font("Calibri", 7.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox1.Location = new System.Drawing.Point(12, 376);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(188, 129);
            this.groupBox1.TabIndex = 5;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Ultimul ziar adaugat";
            // 
            // lastPaperNameLabel
            // 
            this.lastPaperNameLabel.AutoSize = true;
            this.lastPaperNameLabel.Font = new System.Drawing.Font("Calibri", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lastPaperNameLabel.Location = new System.Drawing.Point(6, 18);
            this.lastPaperNameLabel.MaximumSize = new System.Drawing.Size(182, 0);
            this.lastPaperNameLabel.Name = "lastPaperNameLabel";
            this.lastPaperNameLabel.Size = new System.Drawing.Size(171, 24);
            this.lastPaperNameLabel.TabIndex = 5;
            this.lastPaperNameLabel.Text = "Niciun ziar adaugat";
            // 
            // endSaleButton
            // 
            this.endSaleButton.BackColor = System.Drawing.Color.Green;
            this.endSaleButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.endSaleButton.Font = new System.Drawing.Font("Calibri", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.endSaleButton.ForeColor = System.Drawing.Color.White;
            this.endSaleButton.Location = new System.Drawing.Point(206, 376);
            this.endSaleButton.Name = "endSaleButton";
            this.endSaleButton.Size = new System.Drawing.Size(102, 129);
            this.endSaleButton.TabIndex = 6;
            this.endSaleButton.Text = "Incheie vanzarea";
            this.endSaleButton.UseVisualStyleBackColor = false;
            this.endSaleButton.Click += new System.EventHandler(this.endSaleButton_Click);
            // 
            // totalPricePanel
            // 
            this.totalPricePanel.Controls.Add(this.totalPriceLabel);
            this.totalPricePanel.Controls.Add(this.label1);
            this.totalPricePanel.Location = new System.Drawing.Point(-30, 12);
            this.totalPricePanel.Name = "totalPricePanel";
            this.totalPricePanel.Size = new System.Drawing.Size(305, 358);
            this.totalPricePanel.TabIndex = 0;
            // 
            // totalPriceLabel
            // 
            this.totalPriceLabel.Font = new System.Drawing.Font("Consolas", 28.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.totalPriceLabel.Location = new System.Drawing.Point(22, 164);
            this.totalPriceLabel.MaximumSize = new System.Drawing.Size(400, 80);
            this.totalPriceLabel.Name = "totalPriceLabel";
            this.totalPriceLabel.Size = new System.Drawing.Size(280, 80);
            this.totalPriceLabel.TabIndex = 1;
            this.totalPriceLabel.Text = "label2";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Calibri", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(83, 101);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(111, 49);
            this.label1.TabIndex = 0;
            this.label1.Text = "Total:";
            // 
            // simulateKeyboardLabel
            // 
            this.simulateKeyboardLabel.AutoSize = true;
            this.simulateKeyboardLabel.Location = new System.Drawing.Point(183, 559);
            this.simulateKeyboardLabel.Name = "simulateKeyboardLabel";
            this.simulateKeyboardLabel.Size = new System.Drawing.Size(125, 17);
            this.simulateKeyboardLabel.TabIndex = 7;
            this.simulateKeyboardLabel.TabStop = true;
            this.simulateKeyboardLabel.Text = "Simulate keyboard";
            this.simulateKeyboardLabel.VisitedLinkColor = System.Drawing.Color.Blue;
            this.simulateKeyboardLabel.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.simulateKeyboardLabel_LinkClicked);
            // 
            // paperListPanel1
            // 
            this.paperListPanel1.AutoScroll = true;
            this.paperListPanel1.BackColor = System.Drawing.Color.White;
            this.paperListPanel1.Location = new System.Drawing.Point(12, 12);
            this.paperListPanel1.Name = "paperListPanel1";
            this.paperListPanel1.Size = new System.Drawing.Size(296, 358);
            this.paperListPanel1.TabIndex = 2;
            // 
            // MainWnd
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(320, 585);
            this.Controls.Add(this.simulateKeyboardLabel);
            this.Controls.Add(this.totalPricePanel);
            this.Controls.Add(this.endSaleButton);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.paperListPanel1);
            this.Controls.Add(this.paperNameTextbox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "MainWnd";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Monitor Vanzari Ziare";
            this.Load += new System.EventHandler(this.MainWnd_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.totalPricePanel.ResumeLayout(false);
            this.totalPricePanel.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private PaperListPanel paperListPanel1;
        private System.Windows.Forms.Button undoRemovePaperButton;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label lastPaperNameLabel;
        private System.Windows.Forms.Button endSaleButton;
        private System.Windows.Forms.Panel totalPricePanel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label totalPriceLabel;
        private System.Windows.Forms.TextBox paperNameTextbox;
        private System.Windows.Forms.LinkLabel simulateKeyboardLabel;
    }
}

