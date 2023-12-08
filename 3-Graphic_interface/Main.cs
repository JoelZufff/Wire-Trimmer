using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Wire_Trimmer
{
    public partial class Main : Form
    {
        public Main()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            NuevaOrden nuevaOrden = new NuevaOrden();
            nuevaOrden.Show();
        }

        private void ProOr_Click(object sender, EventArgs e)
        {
            Proceso proceso = new Proceso();
            proceso.Show();
        }
    }
}
