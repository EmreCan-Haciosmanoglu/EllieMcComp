using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace Stalker
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.Write(Code("https://www.facebook.com/search/top/?q=stackexchange%20facebook&filters_rp_creation_time=%7B%22start_year%22%3A%222011%22%2C%22end_month%22%3A%222011-01%22%7D&filters_rp_location=155428381137989"));
            Console.Read();
        }
        public static string Code(string Url)
        {
            HttpWebRequest myRequest = (HttpWebRequest)WebRequest.Create(Url);
            myRequest.Method = "GET";
            WebResponse myResponse = myRequest.GetResponse();
            StreamReader sr = new StreamReader(myResponse.GetResponseStream(), System.Text.Encoding.UTF8);
            string result = sr.ReadToEnd();
            sr.Close();
            myResponse.Close();

            return result;
        }
    }
}
