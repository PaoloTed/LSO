import os
import time
import argparse
try:
    from PyPDF2 import PdfReader
    from deep_translator import GoogleTranslator
except ImportError:
    print("Per favore, installa le librerie necessarie eseguendo questo comando:")
    print("pip install PyPDF2 deep-translator")
    exit(1)

def split_text_into_chunks(text, max_chunk_size=4500):
    words = text.split(' ')
    chunks = []
    current_chunk = ""
    for word in words:
        if len(current_chunk) + len(word) + 1 < max_chunk_size:
            current_chunk += word + " "
        else:
            chunks.append(current_chunk.strip())
            current_chunk = word + " "
    if current_chunk:
        chunks.append(current_chunk.strip())
    return chunks

def translate_pdf_to_markdown(pdf_path, output_md_path, start_page=1, num_pages=5, src_lang='en', dest_lang='it'):
    if not os.path.exists(pdf_path):
        print(f"Errore: Il file '{pdf_path}' non esiste.")
        return

    print(f"Leggendo il PDF: {pdf_path}")
    reader = PdfReader(pdf_path)
    translator = GoogleTranslator(source=src_lang, target=dest_lang)
    
    total_pages = len(reader.pages)
    
    end_page = min(start_page + num_pages - 1, total_pages)
    if start_page > total_pages:
        print(f"Errore: Il documento ha solo {total_pages} pagine.")
        return

    print(f"Traduzione delle pagine da {start_page} a {end_page} (su {total_pages} totali).")

    # Aggiungi in coda ('a') tranne se stiamo iniziando da capo e si vuole sovrascrivere
    mode = 'w' if start_page == 1 else 'a'
    
    with open(output_md_path, mode, encoding='utf-8') as md_file:
        if start_page == 1:
            md_file.write(f"# Traduzione del libro: {os.path.basename(pdf_path)}\n\n")
        
        for i in range(start_page - 1, end_page):
            print(f"Traduzione pagina {i+1}/{total_pages} in corso...")
            page = reader.pages[i]
            text = page.extract_text()
            
            md_file.write(f"\n## Pagina {i+1}\n\n")
            
            if text and text.strip():
                chunks = split_text_into_chunks(text)
                for chunk in chunks:
                    try:
                        translated_text = translator.translate(chunk)
                        if translated_text:
                            md_file.write(translated_text + "\n")
                        # Piccola pausa
                        time.sleep(2)
                    except Exception as e:
                        print(f"Errore nella traduzione di un blocco alla pagina {i+1}: {e}")
                        md_file.write(f"\n> [!WARNING]\n> *Errore di traduzione in questo blocco: {e}*\n\n")
            else:
                md_file.write("> *Pagina vuota o contenente solo immagini non testuali.*\n")
                
    print(f"\nTraduzione delle pagine {start_page}-{end_page} completata! Salvato in: {output_md_path}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Traduce un libro PDF a blocchi di pagine.")
    parser.add_argument("input_pdf", help="Percorso del file PDF da tradurre")
    parser.add_argument("output_md", help="Percorso del file Markdown di output")
    parser.add_argument("--start", type=int, default=1, help="Pagina da cui iniziare (default: 1)")
    parser.add_argument("--pages", type=int, default=5, help="Numero di pagine da tradurre in questa esecuzione (default: 5)")
    
    args = parser.parse_args()
    translate_pdf_to_markdown(args.input_pdf, args.output_md, start_page=args.start, num_pages=args.pages)
