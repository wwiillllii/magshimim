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
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void btnLogIn_Click(object sender, EventArgs e)
        {
            string str = txtName.Text + "," + txtPass.Text;
            bool found = false;
            string line;
            if (!File.Exists("Users.txt")) File.Create("Users.txt").Close();
            using(StreamReader sr = new StreamReader("Users.txt"))
                while((line = sr.ReadLine()) != null)
                    if(line == str)
                    {
                        found = true;
                        break;
                    }
            if (!found)
                MessageBox.Show("Incorrect username or password!");
            else
            {
                this.Hide();
                new mainForm(txtName.Text).Show();
            }
        }
    }
}
