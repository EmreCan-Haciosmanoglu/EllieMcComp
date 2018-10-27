using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EllieMcComp
{
    class Task
    {
        public string Name { get => _name; set => _name = value; }
        string _name;

        public string Description { get => _description; set => _description = value; }
        string _description;


        public Task(string name, string description)
        {
            Name = name;
            Description = description;
        }

        public override string ToString()
        {
            return Name + "_" + Description;
        }
    }
}