using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Web;

namespace WebApp
{
    public class Item
    {
        public Bitmap Image { get; set; }
        public int Price { get; set; }
        public string Title { get; set; }
        public string Description { get; set; }

        public Item(string title, int price = 0, string desc = "", Bitmap image = null)
        {
            Title = title;
            Price = price >= 0 ? price : 0;
            Description = desc;
            Image = image;
        }

        public override string ToString()
        {
            return Title + " - " + Price + " - " + Description;
        }
    }
}