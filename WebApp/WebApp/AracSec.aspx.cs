using System;
using System.Collections.Generic;
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

        }

        protected void Make_SelectedIndexChanged(object sender, EventArgs e)
        {
            string id = Make.SelectedValue;
            if (id == "0")
            {
                Model.Enabled = false;
                //Model.Items.Clear();
                //Model.Items.Add(new ListItem("Model Seçin", "0"));
            }
            else
            {
                Model.Enabled = true;
                //Model.Items.Add(new ListItem("Tesla/Ford", "1"));
            }
        }

        protected void Model_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        protected void Year_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        protected void Trim_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        protected void Envanter_Click(object sender, EventArgs e)
        {
            Session["Model"] = Model.SelectedItem.Text;
            Session["Make"] = Make.SelectedItem.Text;
            Session["Year"] = Year.SelectedItem.Text;
            Session["Trim"] = Trim.SelectedItem.Text;
            Response.Redirect("Envanter.aspx");
        }
    }
}