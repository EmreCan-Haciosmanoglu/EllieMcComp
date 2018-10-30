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
    public partial class AddInfo : Form
    {
        Form1 form1;
        public AddInfo(string labelInfo, Form1 form1)
        {
            InitializeComponent();
            label1.Text = labelInfo;
            this.form1 = form1;

        }

        public AddInfo()
        {
            InitializeComponent();
        }

        private void Button1_Click(object sender, EventArgs e)
        {
            if (textBox1.Text == "" || textBox1.Text == null)
            {
                MessageBox.Show("Don't leave text box empty, pls");
                return;
            }
            form1.def = textBox1.Text;
            this.Close();
        }

        private void Button2_Click(object sender, EventArgs e)
        {
            form1.def = null;
            this.Close();
        }
    }
}
