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
            this.label1 = new System.Windows.Forms.Label();
            this.NewO = new System.Windows.Forms.Button();
            this.ProOr = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Constantia", 22F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Underline))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(238, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(320, 54);
            this.label1.TabIndex = 0;
            this.label1.Text = "Wire-Trimmer";
            // 
            // NewO
            // 
            this.NewO.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.NewO.Location = new System.Drawing.Point(290, 139);
            this.NewO.Name = "NewO";
            this.NewO.Size = new System.Drawing.Size(217, 76);
            this.NewO.TabIndex = 1;
            this.NewO.Text = "Nueva orden";
            this.NewO.UseVisualStyleBackColor = true;
            this.NewO.Click += new System.EventHandler(this.button1_Click);
            // 
            // ProOr
            // 
            this.ProOr.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ProOr.Location = new System.Drawing.Point(290, 265);
            this.ProOr.Name = "ProOr";
            this.ProOr.Size = new System.Drawing.Size(217, 76);
            this.ProOr.TabIndex = 2;
            this.ProOr.Text = "Orden(es) en proceso";
            this.ProOr.UseVisualStyleBackColor = true;
            this.ProOr.Click += new System.EventHandler(this.ProOr_Click);
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.GradientInactiveCaption;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.ProOr);
            this.Controls.Add(this.NewO);
            this.Controls.Add(this.label1);
            this.Name = "Main";
            this.Text = " ";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button NewO;
        private System.Windows.Forms.Button ProOr;
    }
}

