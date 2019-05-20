using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data.Common;
using System.Data.SqlClient;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace WebApp
{
    public partial class Envanter : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            if (Session["Model"] == null)
                return;
            SqlConnection cnn = new SqlConnection(ConfigurationManager.ConnectionStrings[0].ConnectionString);

            string sorgu = "SELECT * from Envanter WHERE Model = '" + Session["Model"] + "' AND Make = '" + Session["Make"] + "' AND Year = '" + Session["Year"] + "' AND Trim = '" + Session["Trim"] + "'";
            //string sorgu = "SELECT * from Envanter";
            SqlCommand cmd = new SqlCommand(sorgu, cnn);
            cnn.Open();

            SqlDataReader reader = cmd.ExecuteReader();
            foreach(DbDataRecord d in reader)
            {
                Item i = new Item(d.GetString(0), d.GetInt32(1), d.GetString(2));
                Output.Text = i.ToString();
                break;
            }
            cnn.Close();
        }
    }
}