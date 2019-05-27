using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data.SqlClient;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace WebApp
{
    public partial class AracSec : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            if (!IsPostBack)
            {
                Make.Enabled = true;
                Model.Enabled = false;
                Year.Enabled = false;
                Trim.Enabled = false;

                Make.Items.Clear();
                Make.Items.Add(new ListItem("Marka Seçin", "0"));


                SqlConnection cnn = new SqlConnection(ConfigurationManager.ConnectionStrings[0].ConnectionString);
                cnn.Open();
                SqlCommand cmd = new SqlCommand("SELECT Make FROM Types", cnn);
                SqlDataReader reader = cmd.ExecuteReader();
                List<string> makes = new List<string>();
                int index = 1;
                while (reader.Read())
                {
                    if (!makes.Contains(reader[0].ToString()))
                    {
                        Make.Items.Add(new ListItem(reader[0].ToString(), "" + index));
                        index++;
                        makes.Add(reader[0].ToString());
                    }
                }
                cnn.Close();
                Model.Items.Clear();
                Model.Items.Add(new ListItem("Modeli Seçin", "0"));

                Year.Items.Clear();
                Year.Items.Add(new ListItem("Yılı Seçin", "0"));

                Trim.Items.Clear();
                Trim.Items.Add(new ListItem("Paketi Seçin", "0"));
            }
        }

        protected void Make_SelectedIndexChanged(object sender, EventArgs e)
        {
            Model.Items.Clear();
            Model.Items.Add(new ListItem("Modeli Seçin", "0"));
            Year.Items.Clear();
            Year.Items.Add(new ListItem("Yılı Seçin", "0"));
            Trim.Items.Clear();
            Trim.Items.Add(new ListItem("Paketi Seçin", "0"));

            string id = Make.SelectedValue;
            if (id == "0")
            {
                Model.Enabled = false;
                Year.Enabled = false;
                Trim.Enabled = false;
            }
            else
            {
                Model.Enabled = true;
                SqlConnection cnn = new SqlConnection(ConfigurationManager.ConnectionStrings[0].ConnectionString);
                cnn.Open();
                SqlCommand cmd = new SqlCommand("SELECT Model FROM Types WHERE Make ='" + Make.SelectedItem.Text + "'", cnn);
                SqlDataReader reader = cmd.ExecuteReader();
                List<string> models = new List<string>();
                int index = 1;
                while (reader.Read())
                {
                    if (!models.Contains(reader[0].ToString()))
                    {
                        Model.Items.Add(new ListItem(reader[0].ToString(), "" + index));
                        index++;
                        models.Add(reader[0].ToString());
                    }
                }
                cnn.Close();
            }
        }

        protected void Model_SelectedIndexChanged(object sender, EventArgs e)
        {
            Year.Items.Clear();
            Year.Items.Add(new ListItem("Yılı Seçin", "0"));
            Trim.Items.Clear();
            Trim.Items.Add(new ListItem("Paketi Seçin", "0"));

            string id = Model.SelectedValue;
            if (id == "0")
            {
                Year.Enabled = false;
                Trim.Enabled = false;
            }
            else
            {
                Year.Enabled = true;
                SqlConnection cnn = new SqlConnection(ConfigurationManager.ConnectionStrings[0].ConnectionString);
                cnn.Open();
                SqlCommand cmd = new SqlCommand("SELECT Year FROM Types WHERE Make ='" + Make.SelectedItem.Text + "' AND Model = '" + Model.SelectedItem.Text + "'", cnn);
                SqlDataReader reader = cmd.ExecuteReader();
                List<string> years = new List<string>();
                int index = 1;
                while (reader.Read())
                {
                    if (!years.Contains(reader[0].ToString()))
                    {
                        Year.Items.Add(new ListItem(reader[0].ToString(), "" + index));
                        index++;
                        years.Add(reader[0].ToString());
                    }
                }
                cnn.Close();
            }
        }

        protected void Year_SelectedIndexChanged(object sender, EventArgs e)
        {
            Trim.Items.Clear();
            Trim.Items.Add(new ListItem("Paketi Seçin", "0"));

            string id = Year.SelectedValue;
            if (id == "0")
            {
                Trim.Enabled = false;
            }
            else
            {
                Trim.Enabled = true;
                SqlConnection cnn = new SqlConnection(ConfigurationManager.ConnectionStrings[0].ConnectionString);
                cnn.Open();
                SqlCommand cmd = new SqlCommand("SELECT Trim FROM Types WHERE Make ='" + Make.SelectedItem.Text + "' AND Model = '" + Model.SelectedItem.Text + "' AND Year = '" + Year.SelectedItem.Text + "'", cnn);
                SqlDataReader reader = cmd.ExecuteReader();
                List<string> trims = new List<string>();
                int index = 1;
                while (reader.Read())
                {
                    if (!trims.Contains(reader[0].ToString()))
                    {
                        Trim.Items.Add(new ListItem(reader[0].ToString(), "" + index));
                        index++;
                        trims.Add(reader[0].ToString());
                    }
                }
                cnn.Close();
            }
        }

        protected void Trim_SelectedIndexChanged(object sender, EventArgs e)
        {
            Model.Enabled = true;
            SqlConnection cnn = new SqlConnection(ConfigurationManager.ConnectionStrings[0].ConnectionString);
            cnn.Open();
            SqlCommand cmd = new SqlCommand("SELECT Make FROM Types", cnn);
            SqlDataReader reader = cmd.ExecuteReader();
            List<string> models = new List<string>();
            int index = 1;
            while (reader.Read())
            {
                if (!models.Contains(reader[0].ToString()))
                {
                    Make.Items.Add(new ListItem(reader[0].ToString(), "" + index));
                    index++;
                    models.Add(reader[0].ToString());
                }
            }
            cnn.Close();
        }

        protected void Envanter_Click(object sender, EventArgs e)
        {
            if (Model.SelectedIndex == 0 || Make.SelectedIndex == 0 || Year.SelectedIndex == 0 || Trim.SelectedIndex == 0)
                return;
            Session["Model"] = Model.SelectedItem.Text;
            Session["Make"] = Make.SelectedItem.Text;
            Session["Year"] = Year.SelectedItem.Text;
            Session["Trim"] = Trim.SelectedItem.Text;
            Response.Redirect("Envanter.aspx");
        }
    }
}