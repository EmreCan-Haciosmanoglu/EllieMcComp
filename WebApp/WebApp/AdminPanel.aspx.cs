using System;
using System.Collections.Generic;
using System.Configuration;
using System.IO;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Data.Common;
using System.Data.SqlClient;

namespace WebApp
{
    public partial class AdminPanel : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
        }

        protected void uploadButton_Click(object sender, EventArgs e)
        {
            HttpPostedFile file = imageUpload.PostedFile;
            string name = Path.GetFileName(file.FileName);
            string ext = Path.GetExtension(name);
            int size = file.ContentLength;

            Stream stream = file.InputStream;
            BinaryReader reader = new BinaryReader(stream);
            byte[] bytes = reader.ReadBytes((int)stream.Length);
            SqlConnection cnn = new SqlConnection(ConfigurationManager.ConnectionStrings[0].ConnectionString);

            SqlCommand cmd = new SqlCommand();
            cmd.CommandText = "INSERT INTO Images (ItemID , ImageName , Image) values (1, '" + name + "', @image)";
            cmd.Parameters.AddWithValue("@image", bytes);
            cmd.Connection = cnn;

            cnn.Open();
            cmd.ExecuteNonQuery();
            cnn.Close();

        }

        protected void getImage_Click(object sender, EventArgs e)
        {
            SqlConnection cnn = new SqlConnection(ConfigurationManager.ConnectionStrings[0].ConnectionString);

            string sorgu = "SELECT * from Images";
            SqlCommand cmd = new SqlCommand(sorgu, cnn);
            cnn.Open();
            SqlDataReader reader = cmd.ExecuteReader();
            if (reader.Read())
            {
                byte[] byteData = (byte[])reader[3];
                outImage.ImageUrl = "data:image;base64," + Convert.ToBase64String(byteData);
            }
            cnn.Close();
        }
    }
}