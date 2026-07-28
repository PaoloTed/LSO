import fitz
import os
import sys

pdf_dir = r"g:\Il mio Drive\LSO\PDF Lezioni\Separati"
out_dir = os.path.join(pdf_dir, "extracted_text")
os.makedirs(out_dir, exist_ok=True)

pdfs = sorted([f for f in os.listdir(pdf_dir) if f.lower().endswith('.pdf') and f.startswith('Lezione')])

for pdf_name in pdfs:
    pdf_path = os.path.join(pdf_dir, pdf_name)
    txt_name = pdf_name.replace('.pdf', '.txt')
    txt_path = os.path.join(out_dir, txt_name)
    
    try:
        doc = fitz.open(pdf_path)
        text = ""
        for page in doc:
            text += page.get_text() + "\n---PAGE BREAK---\n"
        doc.close()
        
        with open(txt_path, 'w', encoding='utf-8') as f:
            f.write(text)
        print(f"OK: {pdf_name} -> {len(text)} chars")
    except Exception as e:
        print(f"ERROR: {pdf_name}: {e}")

print(f"\nDone. Extracted {len(pdfs)} files to {out_dir}")
