using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace _2
{
    public partial class Form1 : Form
    {
        private string[] numbers = new string[] { "_2", "_3", "_4", "_5",
            "_6", "_7", "_8", "_9", "_10", "jack", "queen", "king", "ace"};
        private string[] suits = new string[] { "spades", "hearts", "clubs", "diamonds" };
        private string[] pNumbers = new String[] {"02","03","04"
            ,"05","06","07","08","09","10","11","12","13","01"};
        private string[] pSuits = new String[] { "S","H","C","D"};
        private int score = 0;
        private int opponentScore = 0;
        private Random rnd = new Random();
        private TcpClient client = new TcpClient();
        private bool opponentChoseCard = false;
        private string opponentCard = "ace_of_spades";
        private bool cardsDisabled = true;

        public Form1()
        {
            InitializeComponent();
            GenerateCards();
            client.Connect(new IPEndPoint(IPAddress.Parse("127.0.0.1"), 8820));
            Thread t = new Thread(listen);
            t.IsBackground = true;
            t.Start();
        }

        private void GenerateCards()
        {
            for (int x = 0; x < 10; x++)
            {
                PictureBox pb = new PictureBox();
                pb.Image = Properties.Resources.card_back_blue;
                pb.Location = new System.Drawing.Point(10 + 95 * (x % 5), 75 + 130 * (x / 5));
                pb.Name = "pbCard" + x;
                pb.Size = new System.Drawing.Size(85, 120);
                pb.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
                pb.Click += new System.EventHandler(CardClick);
                this.Controls.Add(pb);
            }
        }

        private void CardClick(object obj, EventArgs e)
        {
            if (cardsDisabled) return;
            cardsDisabled = true;
            int num = rnd.Next(numbers.Length);
            int suit = rnd.Next(suits.Length);
            send(1, num,suit);
            string card = numbers[num] + "_of_" + suits[suit];
            Image img = (Image)Properties.Resources.ResourceManager.GetObject(card) ;
            ((PictureBox)obj).Image = img;
            new Thread(delegate()
                {
                    while (!opponentChoseCard) Thread.Sleep(100);
                    opponentChoseCard = false;
                    pbOpponentCard.Image = (Image)
                        Properties.Resources.ResourceManager.GetObject(opponentCard);
                    if (won(card, opponentCard))++score;
                    else if (won(opponentCard, card))++opponentScore;
                    Invoke((MethodInvoker)delegate
                    {
                        lblOwnScore.Text = "Your score: " + score;
                        lblOpponentScore.Text = "Opponent's score: " + opponentScore;
                    });
                    Thread.Sleep(3000);
                    ((PictureBox)obj).Image = Properties.Resources.card_back_blue;
                    pbOpponentCard.Image = Properties.Resources.card_back_red;
                    cardsDisabled = false;
                }).Start();
        }

        private void btnExit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            send(2, 0, 0);
            MessageBox.Show("Your score: " + score + "\nOpponent's score: " + opponentScore);
        }

        private bool won(string card1, string card2)
        {
            int lim1 = card1.IndexOf("_of_");
            int lim2 = card2.IndexOf("_of_");
            if (-1 == lim1 || -1 == lim2) return false;
            card1 = card1.Substring(0, lim1);
            card2 = card2.Substring(0, lim2);
            lim1 = Array.IndexOf(numbers, card1);
            lim2 = Array.IndexOf(numbers, card2);
            if (-1 == lim1 || -1 == lim2) return false;
            return lim1 > lim2;
        }

        private void send(int id, int num, int suit)
        {
            NetworkStream s = client.GetStream();
            s.WriteAsync(new ASCIIEncoding().GetBytes("" + id + pNumbers[num] + pSuits[suit]), 0, 4);
            s.Flush();
        }

        private void listen()
        {
            byte[] buffer = new byte[4];
            int bytesRead = 0;
            string input = "";
            while(true)
            {
                bytesRead = client.GetStream().Read(buffer, 0, 4);
                if (bytesRead != 4) Application.Exit(); 
                input = new ASCIIEncoding().GetString(buffer);
                switch(input[0]-'0')
                {
                    case 0:
                        cardsDisabled = false;
                        break;
                    case 1:
                        int num = Array.IndexOf(pNumbers, input.Substring(1, 2));
                        int suit = Array.IndexOf(pSuits, ""+input[3]);
                        if (-1 == num || -1 == suit)
                            Invoke((MethodInvoker)delegate { Application.Exit(); });
                        opponentCard = numbers[num] + "_of_" + suits[suit];
                        opponentChoseCard = true;
                        break;
                    case 2:
                    default:
                        Application.Exit();
                        break;
                }
            }
        }
    }
}
