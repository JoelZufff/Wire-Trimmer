using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;

namespace Wire_Trimmer
{
    public partial class Main : Form
    {
        DataTable OrdenDataTable = new DataTable();
        //string[] datosP; 
        public int carrete;
        bool con = false;

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
            //mandar el pedido
            int pedido = 0;
            //carrete = 100;
            for(int i = 0;i< OrdenDataTable.Rows.Count;i++)
            {
                int cant=0,lon=0,pel=0;
                cant = int.Parse(OrdenDataTable.Rows[i]["Cantidad"].ToString());
                lon = int.Parse(OrdenDataTable.Rows[i]["Longitud"].ToString());
                pel = int.Parse(OrdenDataTable.Rows[i]["Pelado"].ToString());
                pedido = pedido + (((2 * pel) + lon) * cant);
            }
            if(pedido>carrete)
                MessageBox.Show("Orden incorrecta, cable insuficiente");
            else
            {
                
                MessageBox.Show("Orden correcta");
            }
                
            //NuevaOrden nuevaOrden = new NuevaOrden();
            //nuevaOrden.Show();
        }

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
            if (Cantidad.TextLength == 0 || Longitud.TextLength == 0 || Pelado.TextLength == 0)
                MessageBox.Show("Orden incorrecta");
            else
            {
                OrdenDataTable.Rows.Add(Cantidad.Text, Longitud.Text, Pelado.Text);
                Cantidad.Text = string.Empty;
                Longitud.Text = string.Empty;
                Pelado.Text = "5";
                //Eliminar.Enabled = true;
            }
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

        private void ConectarP_Click(object sender, EventArgs e)
        {
            string[] ports = SerialPort.GetPortNames();
            foreach (string port in ports) 
            {
                serialPort1.PortName = port;
            }
            try
            {
                serialPort1.Open();
                serialPort1.Write("+,00100");
                string[] datosP = serialPort1.ReadExisting().Split(',');
                foreach (string dato in datosP)
                {
                    if (dato.Equals("+"))
                    {
                        Cantidad.Enabled = true;
                        Longitud.Enabled = true;
                        Pelado.Enabled = true;
                        Agregar.Enabled = true;
                        ConectarP.Enabled = false;
                        Eliminar.Enabled = true;
                        con=true;
                    }
                    else if (con == true)
                    {
                        carrete = int.Parse(dato); 
                        if (carrete == 100)
                            MessageBox.Show("carrete enviado exitosamente");
                        //Longitud.Text = carrete.ToString();
                    }
                        //carrete = int.Parse(serialPort1.ReadLine().Substring(1, 5));
                    else if (con==false)
                        MessageBox.Show("No se puede conectar"); 
                }
            }
            catch (Exception)
            {
                if(con==false)
                    MessageBox.Show("No se puede conectar");
                //throw;
            }
        }
    }
}
// 
// recibir la cantidad de cable 
// condicionar el pedido al cable disponible 
// no poder enviar otro pedido hasta que se acabe el que ya se envio con el cable disponible 
// poner el estatus del pedido 
// 