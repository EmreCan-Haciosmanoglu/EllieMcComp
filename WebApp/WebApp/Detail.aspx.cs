using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Configuration;
using System.Data.SqlClient;
using System.Drawing;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.HtmlControls;
using System.Web.UI.WebControls;
using Image = System.Web.UI.WebControls.Image;

namespace WebApp
{
    public partial class Detail : System.Web.UI.Page
    {
        List<Bitmap> bms = new List<Bitmap>();
        List<byte[]> images = new List<byte[]>();
        protected void Page_Load(object sender, EventArgs e)
        {
            string id = Request["id"];
            if (id == null)
            {
                Response.Redirect("AracSec.aspx");
                return;
            }


            SqlConnection cnn = new SqlConnection(ConfigurationManager.ConnectionStrings[0].ConnectionString);
            cnn.Open();
            SqlCommand cmd = new SqlCommand();
            cmd.Connection = cnn;
            cmd.CommandText = "SELECT * FROM Images WHERE ItemID = " + id;
            SqlDataReader reader = cmd.ExecuteReader();

            TypeConverter tc = TypeDescriptor.GetConverter(typeof(Bitmap));
            while (reader.Read())
            {
                bms.Add((Bitmap)tc.ConvertFrom((byte[])reader[1]));
                images.Add((byte[])reader[1]);

                Image smallImage = new Image();
                smallImage.Width = 100;
                smallImage.Height = 100;
                smallImage.ImageAlign = ImageAlign.AbsMiddle;
                smallImage.Attributes.Add("class", "SmallImage");
                smallImage.Attributes.Add("src", "data:image;base64," + Convert.ToBase64String((byte[])reader[1]));
                ImagePrevs_ID.Controls.Add(smallImage);
            }
            reader.Close();

            Image_ID.Attributes.Add("src", "data:image;base64," + Convert.ToBase64String(images[0]));
            if (bms[0].Width > bms[0].Height)
                Image_ID.Attributes.Add("style", "width:inherit");
            else
                Image_ID.Attributes.Add("style", "height:inherit");

            cmd.CommandText = "SELECT * FROM Items WHERE ID = " + id;
            reader = cmd.ExecuteReader();
            reader.Read();

            HtmlGenericControl st = new HtmlGenericControl("strong");
            st.InnerText = reader[2].ToString();
            Title_ID.Controls.Add(st);
            Price_ID.InnerText = reader[3].ToString();
            Desc_ID.InnerText = reader[4].ToString();
            reader.Close();
        }
    }
}