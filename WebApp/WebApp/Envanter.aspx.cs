using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data.Common;
using System.Data.SqlClient;
using System.Web.UI;
using System.Web.UI.HtmlControls;

namespace WebApp
{
    public partial class Envanter : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            if (!IsPostBack)
            {
                Spwn.Controls.Clear();
            }
            if (Session["Model"] == null)
            {
                Response.Redirect("AracSec.aspx");
                return;
            }
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
                item.Attributes.Add("class", "Item_Envanter");

                HtmlGenericControl imageContainer = new HtmlGenericControl("div");
                imageContainer.Attributes.Add("class", "ImageContainer_Div");
                imageContainer.ID = reader[0].ToString();
                imageContainer.Visible = true;
                item.Controls.Add(imageContainer);
                items.Add(imageContainer);
                Spwn.Controls.Add(item);

                HtmlGenericControl buttom = new HtmlGenericControl("div");
                buttom.Attributes.Add("class", "Buttom_Div");

                HtmlGenericControl link = new HtmlGenericControl("a");
                link.Attributes.Add("href", "Detail.aspx?id=" + imageContainer.ID);
                link.Attributes.Add("class", "Link_A");
                link.InnerText = reader[2].ToString();

                HtmlGenericControl linkHover = new HtmlGenericControl("span");
                linkHover.Attributes.Add("class", "ToolTipLink");
                linkHover.InnerText = reader[2].ToString();

                HtmlGenericControl linkContainer = new HtmlGenericControl("div");
                linkContainer.Attributes.Add("class", "LinkContainer_Div");
                linkContainer.Controls.Add(link);
                linkContainer.Controls.Add(linkHover);
                buttom.Controls.Add(linkContainer);

                HtmlButton btn = new HtmlButton();
                btn.Attributes.Add("class", "Button_Ekle");
                btn.InnerText = "Ekle";
                btn.ServerClick += Btn_ServerClick;

                HtmlGenericControl buttonContainer = new HtmlGenericControl("div");
                buttonContainer.Attributes.Add("class", "ButtonContainer_Div");
                buttonContainer.Controls.Add(btn);
                buttom.Controls.Add(buttonContainer);

                item.Controls.Add(buttom);
            }
            reader.Close();
            foreach (HtmlGenericControl item in items)
            {
                cmd.CommandText = "SELECT * FROM Images WHERE ItemID = " + item.ID;
                reader = cmd.ExecuteReader();
                reader.Read();
                byte[] byteData = (byte[])reader[1];

                HtmlGenericControl image = new HtmlGenericControl("img");
                image.Attributes.Add("src", "data:image;base64," + Convert.ToBase64String(byteData));
                image.Attributes.Add("class", "Item_Image");
                item.Controls.Add(image);
            }
            Spwn.Visible = true;
            cnn.Close();
        }

        private void Btn_ServerClick(object sender, EventArgs e)
        {
            ScriptManager.RegisterClientScriptBlock(this, this.GetType(), "alertMessage", "alert('Hi')", true);
        }
    }
}