using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace _1
{
    public partial class mainForm : Form
    {
        string userName;
        public mainForm(string userName)
        {
            InitializeComponent();
            this.userName = userName;
        }

        private void dtp_ValueChanged(object sender, EventArgs e)
        {
            txtUpdate.Text = "";
            string line;
            string date = dtp.Text.Substring(0, dtp.Text.Length - 4);
            string fileName = userName + "BD.txt";
            if (!File.Exists(fileName)) File.Create(fileName).Close();
            using(StreamReader sr = new StreamReader(fileName))
            {
                while((line = sr.ReadLine()) != null)
                    if (line.Contains(date))
                    {
                        lblBirthday.Text = "Birthday of: " + line.Split(',')[0];
                        break;
                    }
                if (line == null)
                    lblBirthday.Text = "No one has a birthday on this day.";
            }
        }

        private void mainForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            Environment.Exit(0);
        }

        private void btnUpdate_Click(object sender, EventArgs e)
        {
            string fileName = userName + "BD.txt";
            string tempFile = Path.GetTempFileName();
            string line;
            string date = dtp.Text.Substring(0, dtp.Text.Length - 4);
            if (!File.Exists(fileName)) File.Create(fileName).Close();
            using (StreamWriter sw = new StreamWriter(tempFile))
            {
                using (StreamReader sr = new StreamReader(fileName))
                    while ((line = sr.ReadLine()) != null)
                        if (!line.Contains(date))
                            sw.WriteLine(line);
                // Removing dates is possible by not writing a name in txtUpdate
                if(!txtUpdate.Text.Trim().Equals(""))
                    sw.WriteLine(txtUpdate.Text + "," + dtp.Text);
            }
            File.Delete(fileName);
            File.Move(tempFile, fileName);
            dtp_ValueChanged(sender, e);
        }
    }
}
