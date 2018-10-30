using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Temp
{
    public partial class MessageBoxTest : Form
    {
        Form1 form1;
        public MessageBoxTest(string message = "", string title = "", Button[] buttons = null)
        {
            InitializeComponent();
            Text = title;
        }

        public MessageBoxTest(List<string> unknownWords, string[] types, Form1 form1)
        {
            this.form1 = form1;

            InitializeComponent();
            foreach (string s in unknownWords)
            {
                cbListUnknownWords.Items.Add(s);
            }
            foreach (string s in types)
            {
                cbListTypes.Items.Add(s);
            }
        }

        private void BtnApply_Click(object sender, EventArgs e)
        {
            if (cbListTypes.CheckedItems.Count != 1)
            {
                MessageBox.Show("Pls select just one type!!!");
                return;
            }
            if (cbListUnknownWords.CheckedItems.Count != 1)
            {
                MessageBox.Show("Pls select just one word!!!");
                return;
            }

            string word = cbListUnknownWords.CheckedItems[0].ToString();
            string type = cbListTypes.CheckedItems[0].ToString();

            form1.Add(word, type);
            form1.unknownWordList.Remove(word);
            this.Close();
        }

        private void BtnCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
