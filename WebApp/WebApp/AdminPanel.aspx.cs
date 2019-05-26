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

        protected void Btn_Upload_Click(object sender, EventArgs e)
        {
            SqlConnection cnn = new SqlConnection(ConfigurationManager.ConnectionStrings[0].ConnectionString);
            cnn.Open();

            string qTypes = "INSERT INTO Types (Make, Model, Year, Trim) values ('" + tb_Make.Text + "', '" + tb_Model.Text + "', " + tb_Year.Text + ", '" + tb_Trim.Text + "')";
            SqlCommand cmd = new SqlCommand(qTypes, cnn);
            cmd.ExecuteNonQuery();

            string qGetIDBack = "SELECT SCOPE_IDENTITY()";
            cmd.CommandText = qGetIDBack;

            int qTypesID = int.Parse(cmd.ExecuteScalar().ToString());

            string qItems = "INSERT INTO Items ( TypeID, Title, Price, Description) values ( " + qTypesID + ", '" + tb_Title.Text + "', " + tb_Price.Text + ", '" + tb_Description.Text + "')";
            cmd.CommandText = qItems;
            cmd.ExecuteNonQuery();

            cmd.CommandText = qGetIDBack;
            int qItemsID = int.Parse(cmd.ExecuteScalar().ToString());


            List<HttpPostedFile> files = imageUpload.PostedFiles.ToList();
            foreach (HttpPostedFile file in files)
            {
                string name = Path.GetFileName(file.FileName);
                string ext = Path.GetExtension(name);
                int size = file.ContentLength;

                Stream stream = file.InputStream;
                BinaryReader reader = new BinaryReader(stream);
                byte[] bytes = reader.ReadBytes((int)stream.Length);

                SqlCommand cmdImages = new SqlCommand();
                cmdImages.CommandText = "INSERT INTO Images (ItemID , Image) values (" + qItemsID + ", @image)";
                cmdImages.Parameters.AddWithValue("@image", bytes);
                cmdImages.Connection = cnn;

                cmdImages.ExecuteNonQuery();
            }
            cnn.Close();
        }
    }
}