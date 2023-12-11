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
        DataTable OrdenDataTable = new DataTable();

        public Main()
        {
            InitializeComponent();
            Ordenes.Show();
            Ordenes.DataSource = OrdenDataTable;
        }

        private void Main_Load(object sender, EventArgs e)
        {
            OrdenDataTable.Columns.Add("Cantidad", typeof(string));
            OrdenDataTable.Columns.Add("Longitud", typeof(string));
            OrdenDataTable.Columns.Add("Pelado", typeof(string));
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //NuevaOrden nuevaOrden = new NuevaOrden();
            //nuevaOrden.Show();
        }

        //private void ProOr_Click(object sender, EventArgs e)
        //{
           // Proceso proceso = new Proceso();
            //proceso.Show();
        //}

        private void Cantidad_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
                Agregar.PerformClick();
            else if ((e.KeyChar >= 32 && e.KeyChar <= 47) || (e.KeyChar >= 58))
                e.Handled = true;
        }

        private void Longitud_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
                Agregar.PerformClick();
            else if ((e.KeyChar >= 32 && e.KeyChar <= 47) || (e.KeyChar >= 58))
                e.Handled = true;
        }

        private void Pelado_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
                Agregar.PerformClick();
            else if ((e.KeyChar >= 32 && e.KeyChar <= 47) || (e.KeyChar >= 58))
                e.Handled = true;
        }

        private void Agregar_Click(object sender, EventArgs e)
        {
            OrdenDataTable.Rows.Add(Cantidad.Text, Longitud.Text, Pelado.Text);
            Cantidad.Text = string.Empty;
            Longitud.Text = string.Empty;
            Pelado.Text = "5";

        }

        private void Eliminar_Click(object sender, EventArgs e)
        {
            if (OrdenDataTable.Rows.Count == 0)
                return;
            DialogResult Answer = MessageBox.Show("¿Esta seguro que desea eliminar la orden seleccionada?", "Confirmacion", MessageBoxButtons.YesNo, MessageBoxIcon.Exclamation);
            if (Answer == DialogResult.No)
                return;

            // Eliminar producto de lista
            string Order = Ordenes.CurrentRow.Cells[0].Value.ToString();

            // Eliminamos producto en DataTable mostrado
            foreach (DataRow Row in OrdenDataTable.Rows)
                if (Row[0].ToString() == Order) { Row.Delete(); break; }
        }
    }
}
// que solo agrege el pedido si hay datos 
// 