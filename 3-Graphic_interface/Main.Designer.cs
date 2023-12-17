namespace Wire_Trimmer
{
    partial class Main
    {
        /// <summary>
        /// Variable del diseñador necesaria.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Limpiar los recursos que se estén usando.
        /// </summary>
        /// <param name="disposing">true si los recursos administrados se deben desechar; false en caso contrario.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Código generado por el Diseñador de Windows Forms

        /// <summary>
        /// Método necesario para admitir el Diseñador. No se puede modificar
        /// el contenido de este método con el editor de código.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
            this.Title = new System.Windows.Forms.Label();
            this.Imprimir = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.Ordenes = new System.Windows.Forms.DataGridView();
            this.AddButton = new System.Windows.Forms.Button();
            this.EraseButton = new System.Windows.Forms.Button();
            this.SerialPort = new System.IO.Ports.SerialPort(this.components);
            this.PortsComboBox = new System.Windows.Forms.ComboBox();
            this.TimerSerialPort = new System.Windows.Forms.Timer(this.components);
            this.WireOrderPanel = new System.Windows.Forms.Panel();
            this.PeelingLength = new System.Windows.Forms.NumericUpDown();
            this.Length = new System.Windows.Forms.NumericUpDown();
            this.Amount = new System.Windows.Forms.NumericUpDown();
            this.ConectionPanel = new System.Windows.Forms.Panel();
            this.ConnectionTitle = new System.Windows.Forms.Label();
            this.ConectionButton = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.Ordenes)).BeginInit();
            this.WireOrderPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.PeelingLength)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.Length)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.Amount)).BeginInit();
            this.ConectionPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // Title
            // 
            this.Title.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.Title.AutoSize = true;
            this.Title.Font = new System.Drawing.Font("Corbel", 27.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Title.Location = new System.Drawing.Point(228, 10);
            this.Title.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.Title.Name = "Title";
            this.Title.Size = new System.Drawing.Size(241, 45);
            this.Title.TabIndex = 0;
            this.Title.Text = "Wire-Trimmer";
            // 
            // Imprimir
            // 
            this.Imprimir.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.Imprimir.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Imprimir.Location = new System.Drawing.Point(273, 510);
            this.Imprimir.Margin = new System.Windows.Forms.Padding(2);
            this.Imprimir.Name = "Imprimir";
            this.Imprimir.Size = new System.Drawing.Size(145, 49);
            this.Imprimir.TabIndex = 1;
            this.Imprimir.Text = "Imprimir";
            this.Imprimir.UseVisualStyleBackColor = true;
            this.Imprimir.Click += new System.EventHandler(this.button1_Click);
            // 
            // label2
            // 
            this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(13, 75);
            this.label2.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(145, 20);
            this.label2.TabIndex = 6;
            this.label2.Text = "Cantidad de cables";
            // 
            // label3
            // 
            this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(183, 75);
            this.label3.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(111, 20);
            this.label3.TabIndex = 7;
            this.label3.Text = "Longitud (mm)";
            // 
            // label4
            // 
            this.label4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(336, 75);
            this.label4.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(98, 20);
            this.label4.TabIndex = 8;
            this.label4.Text = "Pelado (mm)";
            // 
            // Ordenes
            // 
            this.Ordenes.AllowUserToAddRows = false;
            this.Ordenes.AllowUserToDeleteRows = false;
            dataGridViewCellStyle2.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle2.BackColor = System.Drawing.SystemColors.Control;
            dataGridViewCellStyle2.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            dataGridViewCellStyle2.ForeColor = System.Drawing.SystemColors.WindowText;
            dataGridViewCellStyle2.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle2.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle2.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.Ordenes.AlternatingRowsDefaultCellStyle = dataGridViewCellStyle2;
            this.Ordenes.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.Ordenes.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.Fill;
            this.Ordenes.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.Ordenes.Location = new System.Drawing.Point(17, 154);
            this.Ordenes.Margin = new System.Windows.Forms.Padding(2);
            this.Ordenes.Name = "Ordenes";
            this.Ordenes.ReadOnly = true;
            this.Ordenes.RowHeadersVisible = false;
            this.Ordenes.RowHeadersWidth = 62;
            this.Ordenes.RowTemplate.Height = 28;
            this.Ordenes.Size = new System.Drawing.Size(667, 338);
            this.Ordenes.TabIndex = 9;
            // 
            // AddButton
            // 
            this.AddButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.AddButton.Enabled = false;
            this.AddButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.AddButton.Location = new System.Drawing.Point(476, 97);
            this.AddButton.Margin = new System.Windows.Forms.Padding(2);
            this.AddButton.Name = "AddButton";
            this.AddButton.Size = new System.Drawing.Size(91, 32);
            this.AddButton.TabIndex = 10;
            this.AddButton.Text = "Agregar";
            this.AddButton.UseVisualStyleBackColor = true;
            this.AddButton.Click += new System.EventHandler(this.Agregar_Click);
            // 
            // EraseButton
            // 
            this.EraseButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.EraseButton.Enabled = false;
            this.EraseButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.EraseButton.Location = new System.Drawing.Point(600, 97);
            this.EraseButton.Margin = new System.Windows.Forms.Padding(2);
            this.EraseButton.Name = "EraseButton";
            this.EraseButton.Size = new System.Drawing.Size(91, 32);
            this.EraseButton.TabIndex = 11;
            this.EraseButton.Text = "Eliminar";
            this.EraseButton.UseVisualStyleBackColor = true;
            this.EraseButton.Click += new System.EventHandler(this.Eliminar_Click);
            // 
            // SerialPort
            // 
            this.SerialPort.PortName = "COM3";
            this.SerialPort.ReadTimeout = 500;
            this.SerialPort.WriteTimeout = 500;
            this.SerialPort.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.SerialPort_DataReceived);
            // 
            // PortsComboBox
            // 
            this.PortsComboBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PortsComboBox.FormattingEnabled = true;
            this.PortsComboBox.Location = new System.Drawing.Point(226, 246);
            this.PortsComboBox.Name = "PortsComboBox";
            this.PortsComboBox.Size = new System.Drawing.Size(250, 32);
            this.PortsComboBox.TabIndex = 13;
            this.PortsComboBox.MouseClick += new System.Windows.Forms.MouseEventHandler(this.PortsComboBox_MouseClick);
            // 
            // TimerSerialPort
            // 
            this.TimerSerialPort.Interval = 10;
            this.TimerSerialPort.Tick += new System.EventHandler(this.TimerSerialPort_Tick);
            // 
            // WireOrderPanel
            // 
            this.WireOrderPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)));
            this.WireOrderPanel.Controls.Add(this.PeelingLength);
            this.WireOrderPanel.Controls.Add(this.Length);
            this.WireOrderPanel.Controls.Add(this.Imprimir);
            this.WireOrderPanel.Controls.Add(this.Ordenes);
            this.WireOrderPanel.Controls.Add(this.Amount);
            this.WireOrderPanel.Controls.Add(this.Title);
            this.WireOrderPanel.Controls.Add(this.EraseButton);
            this.WireOrderPanel.Controls.Add(this.label2);
            this.WireOrderPanel.Controls.Add(this.AddButton);
            this.WireOrderPanel.Controls.Add(this.label3);
            this.WireOrderPanel.Controls.Add(this.label4);
            this.WireOrderPanel.Location = new System.Drawing.Point(-10, 12);
            this.WireOrderPanel.Name = "WireOrderPanel";
            this.WireOrderPanel.Size = new System.Drawing.Size(705, 561);
            this.WireOrderPanel.TabIndex = 14;
            this.WireOrderPanel.Visible = false;
            // 
            // PeelingLength
            // 
            this.PeelingLength.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.PeelingLength.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PeelingLength.Location = new System.Drawing.Point(340, 101);
            this.PeelingLength.Name = "PeelingLength";
            this.PeelingLength.Size = new System.Drawing.Size(96, 29);
            this.PeelingLength.TabIndex = 17;
            // 
            // Length
            // 
            this.Length.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.Length.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Length.Location = new System.Drawing.Point(187, 102);
            this.Length.Name = "Length";
            this.Length.Size = new System.Drawing.Size(96, 29);
            this.Length.TabIndex = 16;
            // 
            // Amount
            // 
            this.Amount.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.Amount.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Amount.Location = new System.Drawing.Point(38, 100);
            this.Amount.Name = "Amount";
            this.Amount.Size = new System.Drawing.Size(96, 29);
            this.Amount.TabIndex = 15;
            // 
            // ConectionPanel
            // 
            this.ConectionPanel.Controls.Add(this.ConectionButton);
            this.ConectionPanel.Controls.Add(this.ConnectionTitle);
            this.ConectionPanel.Controls.Add(this.PortsComboBox);
            this.ConectionPanel.Location = new System.Drawing.Point(0, 2);
            this.ConectionPanel.Name = "ConectionPanel";
            this.ConectionPanel.Size = new System.Drawing.Size(681, 571);
            this.ConectionPanel.TabIndex = 15;
            // 
            // ConnectionTitle
            // 
            this.ConnectionTitle.AutoSize = true;
            this.ConnectionTitle.Font = new System.Drawing.Font("Corbel", 24F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ConnectionTitle.Location = new System.Drawing.Point(108, 154);
            this.ConnectionTitle.Name = "ConnectionTitle";
            this.ConnectionTitle.Size = new System.Drawing.Size(476, 39);
            this.ConnectionTitle.TabIndex = 14;
            this.ConnectionTitle.Text = "CONEXION CON WIRE-TRIMMER";
            // 
            // ConectionButton
            // 
            this.ConectionButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ConectionButton.Location = new System.Drawing.Point(246, 328);
            this.ConectionButton.Name = "ConectionButton";
            this.ConectionButton.Size = new System.Drawing.Size(190, 52);
            this.ConectionButton.TabIndex = 15;
            this.ConectionButton.Text = "Solicitar conexion";
            this.ConectionButton.UseVisualStyleBackColor = true;
            this.ConectionButton.Click += new System.EventHandler(this.ConectionButton_Click);
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.GradientInactiveCaption;
            this.ClientSize = new System.Drawing.Size(684, 576);
            this.Controls.Add(this.ConectionPanel);
            this.Controls.Add(this.WireOrderPanel);
            this.Margin = new System.Windows.Forms.Padding(2);
            this.MinimumSize = new System.Drawing.Size(700, 615);
            this.Name = "Main";
            this.Text = " ";
            this.Load += new System.EventHandler(this.Main_Load);
            ((System.ComponentModel.ISupportInitialize)(this.Ordenes)).EndInit();
            this.WireOrderPanel.ResumeLayout(false);
            this.WireOrderPanel.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.PeelingLength)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.Length)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.Amount)).EndInit();
            this.ConectionPanel.ResumeLayout(false);
            this.ConectionPanel.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label Title;
        private System.Windows.Forms.Button Imprimir;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.DataGridView Ordenes;
        private System.Windows.Forms.Button AddButton;
        private System.Windows.Forms.Button EraseButton;
        private System.IO.Ports.SerialPort SerialPort;
        private System.Windows.Forms.ComboBox PortsComboBox;
        private System.Windows.Forms.Timer TimerSerialPort;
        private System.Windows.Forms.Panel WireOrderPanel;
        private System.Windows.Forms.NumericUpDown Amount;
        private System.Windows.Forms.NumericUpDown PeelingLength;
        private System.Windows.Forms.NumericUpDown Length;
        private System.Windows.Forms.Panel ConectionPanel;
        private System.Windows.Forms.Label ConnectionTitle;
        private System.Windows.Forms.Button ConectionButton;
    }
}

