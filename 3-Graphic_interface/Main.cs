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

namespace Wire_Trimmer
{
    public partial class Main : Form
    {
        // DataTable para tabla dinamica
        DataTable OrdenDataTable = new DataTable();

        // Variables Globales
        public int      Reel;
        public bool     ConectionStatus = false;
        string          Data = string.Empty;

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
            //Reel = 100;
            for(int i = 0;i< OrdenDataTable.Rows.Count;i++)
            {
                int cant = 0,lon = 0,pel = 0;
                cant = int.Parse(OrdenDataTable.Rows[i]["Cantidad"].ToString());
                lon = int.Parse(OrdenDataTable.Rows[i]["Longitud"].ToString());
                pel = int.Parse(OrdenDataTable.Rows[i]["Pelado"].ToString());
                pedido = pedido + (((2 * pel) + lon) * cant);
            }
            if(pedido>Reel)
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
                AddButton.PerformClick();
            else if ((e.KeyChar >= 32 && e.KeyChar <= 47) || (e.KeyChar >= 58))
                e.Handled = true;
        }

        private void Longitud_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
                AddButton.PerformClick();
            else if ((e.KeyChar >= 32 && e.KeyChar <= 47) || (e.KeyChar >= 58))
                e.Handled = true;
        }

        private void Pelado_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
                AddButton.PerformClick();
            else if ((e.KeyChar >= 32 && e.KeyChar <= 47) || (e.KeyChar >= 58))
                e.Handled = true;
        }

        private void Agregar_Click(object sender, EventArgs e)
        {
            OrdenDataTable.Rows.Add(Amount.Text, Length.Text, PeelingLength.Text);
            Amount.Text = string.Empty;
            Length.Text = string.Empty;
            PeelingLength.Text = "5";
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


        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            if(SerialPort.IsOpen)
                Data = SerialPort.ReadExisting();
        }

        private void TimerSerialPort_Tick(object sender, EventArgs e)
        {
            
        }

        private void PortsComboBox_MouseClick(object sender, MouseEventArgs e)
        {
            string[] Ports = SerialPort.GetPortNames();

            PortsComboBox.Items.Clear();
            PortsComboBox.Items.AddRange(Ports);
            PortsComboBox.SelectedItem = 0;
        }

        private void ConectionButton_Click(object sender, EventArgs e)
        {
            try
            {
                SerialPort.PortName = PortsComboBox.Text;
                SerialPort.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
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



/*foreach (string Port in Ports) 
    SerialPort.PortName = Port;

try
{
    SerialPort.Open();
    SerialPort.Write("+,00100");
    string[] datosP = SerialPort.ReadExisting().Split(',');
    foreach (string dato in datosP)
    {
        if (dato.Equals("+"))
        {
            Cantidad.Enabled = true;
            Longitud.Enabled = true;
            Pelado.Enabled = true;
            Agregar.Enabled = true;
            ConectionButton.Enabled = false;
            Eliminar.Enabled = true;
            ConectionStatus = true;
        }
        else if (ConectionStatus == true)
        {
            Reel = int.Parse(dato); 
            if (Reel == 100)
                MessageBox.Show("Carrete enviado exitosamente");
            //Longitud.Text = Reel.ToString();
        }
            //Reel = int.Parse(serialPort1.ReadLine().Substring(1, 5));
        else if ( ConectionStatus == false )
            MessageBox.Show("No se puede conectar"); 
    }
}
catch (Exception)
{
    if( ConectionStatus == false )
        MessageBox.Show("No se puede conectar");
    //throw;
            }*/