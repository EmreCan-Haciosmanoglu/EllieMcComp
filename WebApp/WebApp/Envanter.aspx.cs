using System;
using System.Configuration;
using System.Data.Common;
using System.Data.SqlClient;

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
            SqlCommand cmd = new SqlCommand(sorgu, cnn);
            cnn.Open();

            SqlDataReader reader = cmd.ExecuteReader();
            int IDCount = 0;
            foreach(DbDataRecord d in reader)
            {
                Item i = new Item(d.GetString(0), d.GetInt32(1), d.GetString(2));
                System.Web.UI.HtmlControls.HtmlGenericControl item = new System.Web.UI.HtmlControls.HtmlGenericControl();
                item.ID = "item_" + IDCount;
                item.Style.Value = "background-color:gray; height:100px; margin:40px; padding:10px;";
                item.Visible = true;
                item.Attributes.Add("class", "itemclass_" + IDCount);
                Spwn.Controls.Add(item);
                IDCount++;
            }
            Spwn.Visible = true;
            cnn.Close();
        }
    }
}