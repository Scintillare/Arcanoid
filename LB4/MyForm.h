#pragma once
#include "Arcanoid.h"

namespace LB4 {

  using namespace System;
  using namespace System::ComponentModel;
  using namespace System::Collections;
  using namespace System::Windows::Forms;
  using namespace System::Data;
  using namespace System::Drawing;

  /// <summary>
  /// Summary for MyForm
  /// </summary>
  public ref class MyForm : public System::Windows::Forms::Form
  {
  public:
    MyForm(void)
    {
      InitializeComponent();
      System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
      // 
      // MyField
      // 
      GameOver = false;
      my_field = gcnew Field(pictureBox1);
      my_field->push_back(gcnew Ball(PointF(351, 430), Drawing::SizeF(20, 20), Brushes::Crimson, 270*(System::Math::PI/180), 1));
      my_field->push_back(gcnew Platform(PointF(320, 450), Drawing::SizeF(125, 20), Brushes::DarkViolet));
      my_field->push_back(gcnew Border(PointF(0, 0), Drawing::SizeF((float)pictureBox1->Size.Width, 2), Brushes::Transparent, 1));
      my_field->push_back(gcnew Border(PointF(0, 0), Drawing::SizeF(2, (float)pictureBox1->Size.Height), Brushes::Transparent, 1));
      my_field->push_back(gcnew Border(PointF((float)pictureBox1->Size.Width - 2, 0), Drawing::SizeF(2, (float)pictureBox1->Size.Height), Brushes::Transparent, 1));
      my_field->push_back(gcnew Border(PointF(0, (float)pictureBox1->Size.Height), Drawing::SizeF((float)pictureBox1->Size.Width, 2), Brushes::Transparent, 0));
      my_field->push_back(gcnew BlocksLine(PointF(2, 50), Drawing::SizeF(85, 40), (cli::safe_cast<System::Drawing::Image^> (resources->GetObject(L"YellowBlock"))), 2, pictureBox1));
      my_field->push_back(gcnew BlocksLine(PointF(2, 100), Drawing::SizeF(85, 40), (cli::safe_cast<System::Drawing::Image^> (resources->GetObject(L"BlueBlock"))), 2, pictureBox1));
      my_field->push_back(gcnew BlocksLine(PointF(2, 150), Drawing::SizeF(85, 40), (cli::safe_cast<System::Drawing::Image^> (resources->GetObject(L"YellowBlock"))), 2, pictureBox1));
      //
      //TODO: Add the constructor code here
      //
    }

  protected:
    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    ~MyForm()
    {
      if (components)
      {
        delete components;
      }
    }
  private: System::Windows::Forms::PictureBox^  pictureBox1;
  private: Field^ my_field;
  private: bool GameOver;
  private: System::Windows::Forms::Timer^  timer1;
  private: System::ComponentModel::IContainer^  components;
  protected:

  private:
    /// <summary>
    /// Required designer variable.
    /// </summary>

#pragma region Windows Form Designer generated code
    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    void InitializeComponent(void)
    {
      this->components = (gcnew System::ComponentModel::Container());
      this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
      this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
      this->SuspendLayout();
      // 
      // timer1
      // 
      this->timer1->Tick += gcnew System::EventHandler(this, &MyForm::timer1_Tick);
      // 
      // pictureBox1
      // 
      System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
      this->pictureBox1->Dock = System::Windows::Forms::DockStyle::Fill;
      this->pictureBox1->Location = System::Drawing::Point(0, 0);
      this->pictureBox1->Name = L"pictureBox1";
      this->pictureBox1->Size = System::Drawing::Size(700, 500);
      this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::CenterImage;
      this->pictureBox1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^> (resources->GetObject(L"BackGroundCat")));
      this->pictureBox1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
      this->pictureBox1->TabIndex = 0;
      this->pictureBox1->TabStop = false;
      this->pictureBox1->Click += gcnew System::EventHandler(this, &MyForm::pictureBox1_Click);
      this->pictureBox1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::pictureBox1_MouseMove);
      // 
      // MyForm
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(700, 500);
      this->Controls->Add(this->pictureBox1);
      this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
      this->KeyPreview = true;
      this->Name = L"MyForm";
      this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
      this->Text = L"Arcanoid";
      this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
      this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::MyForm_KeyPress);
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
      this->ResumeLayout(false);

    }

#pragma endregion

  private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
    System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
    Graphics^ gr = pictureBox1->CreateGraphics(); if (my_field->isDeath()) {
      GameOver = true;
      gr->DrawImage(gcnew Bitmap((cli::safe_cast<System::Drawing::Image^> (resources->GetObject(L"SadCatBack")))), Point (0, 0));
    }
    else {
      if (my_field->isWin()) {
        gr->DrawImage(gcnew Bitmap((cli::safe_cast<System::Drawing::Image^> (resources->GetObject(L"WinVarenik")))), Point(0, 0));
      }
      else {
        my_field->Tick();
        my_field->Draw();
      }
    }
  }
  private: System::Void pictureBox1_Click(System::Object^  sender, System::EventArgs^  e) {
    timer1->Enabled = !timer1->Enabled;
  }
  private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
    timer1->Interval = 10;
    System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
    pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^> (resources->GetObject(L"BagCatBackBeige")));
  }
  private: System::Void MyForm_KeyPress(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
   if (!GameOver) {
      my_field->MovePlatform(e);
      if (e->KeyCode == Keys::Enter) {
        timer1->Enabled = !timer1->Enabled;
      }
    }
  }
  private: System::Void pictureBox1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
   if (!GameOver) {
     my_field->MovePlatform(e);
   }
  }
 };
}

