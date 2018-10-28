using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace EllieMcComp
{
    class Project
    {
        public static Dictionary<string, Project> projects = new Dictionary<string, Project>();
        public string Name { get => _name; set => _name = value; }
        string _name;

        public string Path { get => _path; set => _path = value; }
        string _path;

        public string Description { get => _description; set => _description = value; }
        string _description;

        public Project(string name, string description, string path)
        {
            Name = name;
            Path = path;
            Description = description;
        }

        public override string ToString()
        {
            return Name + "" + Description + "" + Path;
        }
    }
}