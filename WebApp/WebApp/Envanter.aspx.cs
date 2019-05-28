using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data.Common;
using System.Data.SqlClient;
using System.Web.UI.HtmlControls;

namespace WebApp
{
    public partial class Envanter : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            if(!IsPostBack)
            {
                Spwn.Controls.Clear();
            }
            if (Session["Model"] == null)
                return;
            List<HtmlGenericControl> items = new List<HtmlGenericControl>();
            SqlConnection cnn = new SqlConnection(ConfigurationManager.ConnectionStrings[0].ConnectionString);
            cnn.Open();

            string query = "SELECT ID FROM Types WHERE Model = '" + Session["Model"] + "' AND Make = '" + Session["Make"] + "' AND Year = '" + Session["Year"] + "' AND Trim = '" + Session["Trim"] + "'";
            SqlCommand cmd = new SqlCommand(query, cnn);
            int id = int.Parse(cmd.ExecuteScalar().ToString());

            cmd.CommandText = "SELECT * FROM Items WHERE TypeID = " + id;
            SqlDataReader reader = cmd.ExecuteReader();

            while (reader.Read())
            {
                HtmlGenericControl item = new HtmlGenericControl("div");
                item.ID = reader[0].ToString();
                item.Style.Value = "margin:40px; padding:10px; border:2px; bordor-color:gray;";
                item.Visible = true;
                item.Attributes.Add("class", "itemclass_" + item.ID);
                items.Add(item);
            }
            reader.Close();
            foreach(HtmlGenericControl item in items)
            {
                cmd.CommandText = "SELECT * FROM Images WHERE ItemID = " + item.ID;
                reader = cmd.ExecuteReader();
                reader.Read();
                byte[] byteData = (byte[])reader[1];

                HtmlGenericControl image = new HtmlGenericControl("img");
                image.Attributes.Add("src", "data:image;base64," + Convert.ToBase64String(byteData));

                item.Controls.Add(image);
                Spwn.Controls.Add(item);
            }
            Spwn.Visible = true;
            cnn.Close();
        }
    }
}