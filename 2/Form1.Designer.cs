namespace _2
{
    partial class Form1
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.lblOwnScore = new System.Windows.Forms.Label();
            this.lblOpponentScore = new System.Windows.Forms.Label();
            this.btnExit = new System.Windows.Forms.Button();
            this.pbOpponentCard = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.pbOpponentCard)).BeginInit();
            this.SuspendLayout();
            // 
            // lblOwnScore
            // 
            this.lblOwnScore.AutoSize = true;
            this.lblOwnScore.Location = new System.Drawing.Point(13, 13);
            this.lblOwnScore.Name = "lblOwnScore";
            this.lblOwnScore.Size = new System.Drawing.Size(70, 13);
            this.lblOwnScore.TabIndex = 0;
            this.lblOwnScore.Text = "Your score: 0";
            // 
            // lblOpponentScore
            // 
            this.lblOpponentScore.AutoSize = true;
            this.lblOpponentScore.Location = new System.Drawing.Point(16, 30);
            this.lblOpponentScore.Name = "lblOpponentScore";
            this.lblOpponentScore.Size = new System.Drawing.Size(102, 13);
            this.lblOpponentScore.TabIndex = 1;
            this.lblOpponentScore.Text = "Opponent\'s score: 0";
            // 
            // btnExit
            // 
            this.btnExit.Location = new System.Drawing.Point(239, 13);
            this.btnExit.Name = "btnExit";
            this.btnExit.Size = new System.Drawing.Size(75, 30);
            this.btnExit.TabIndex = 2;
            this.btnExit.Text = "Exit";
            this.btnExit.UseVisualStyleBackColor = true;
            this.btnExit.Click += new System.EventHandler(this.btnExit_Click);
            // 
            // pbOpponentCard
            // 
            this.pbOpponentCard.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.pbOpponentCard.Image = ((System.Drawing.Image)(resources.GetObject("pbOpponentCard.Image")));
            this.pbOpponentCard.Location = new System.Drawing.Point(486, 12);
            this.pbOpponentCard.Name = "pbOpponentCard";
            this.pbOpponentCard.Size = new System.Drawing.Size(85, 120);
            this.pbOpponentCard.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pbOpponentCard.TabIndex = 3;
            this.pbOpponentCard.TabStop = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(583, 361);
            this.Controls.Add(this.pbOpponentCard);
            this.Controls.Add(this.btnExit);
            this.Controls.Add(this.lblOpponentScore);
            this.Controls.Add(this.lblOwnScore);
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.pbOpponentCard)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lblOwnScore;
        private System.Windows.Forms.Label lblOpponentScore;
        private System.Windows.Forms.Button btnExit;
        private System.Windows.Forms.PictureBox pbOpponentCard;
    }
}

