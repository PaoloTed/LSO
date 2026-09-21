import os
import json
import time
import re
from deep_translator import GoogleTranslator

def extract_book_from_transcript(transcript_path):
    with open(transcript_path, 'r', encoding='utf-8') as f:
        for line in f:
            try:
                data = json.loads(line)
                content = data.get('content', '')
                if '==Start of PDF==' in content and len(content) > 10000:
                    start_idx = content.find('==Start of PDF==')
                    end_idx = content.find('==End of PDF==')
                    if end_idx != -1:
                        return content[start_idx:end_idx]
                    else:
                        return content[start_idx:]
            except Exception:
                pass
    return None

def parse_pages(book_raw_text):
    pages = []
    pattern = re.compile(r'==Start of OCR for page (\d+)==(.*?)==End of OCR for page \1==', re.DOTALL)
    for match in pattern.finditer(book_raw_text):
        page_num = match.group(1)
        page_text = match.group(2).strip()
        pages.append((page_num, page_text))
    return pages

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

def translate_pages(pages, output_md_path, src_lang='en', dest_lang='it'):
    translator = GoogleTranslator(source=src_lang, target=dest_lang)
    
    with open(output_md_path, 'w', encoding='utf-8') as md_file:
        md_file.write(f"# Traduzione del libro The Body & Society\n\n")
        
        for page_num, text in pages:
            print(f"Traduzione pagina {page_num} in corso...")
            md_file.write(f"\n## Pagina {page_num}\n\n")
            
            if text:
                chunks = split_text_into_chunks(text)
                for chunk in chunks:
                    try:
                        translated_text = translator.translate(chunk)
                        if translated_text:
                            md_file.write(translated_text + "\n")
                        # Pausa per l'API gratuita
                        time.sleep(0.5)
                    except Exception as e:
                        print(f"Errore nella traduzione di un blocco alla pagina {page_num}: {e}")
                        md_file.write(f"\n> [!WARNING]\n> *Errore di traduzione in questo blocco: {e}*\n\n")
            else:
                md_file.write("> *Pagina vuota o contenente solo immagini non testuali.*\n")

if __name__ == "__main__":
    transcript_path = r"C:\Users\Paolo\.gemini\antigravity-ide\brain\0d9d65d3-7b4d-4099-8966-e70a46214b1d\.system_generated\logs\transcript_full.jsonl"
    output_md_path = r"C:\Users\Paolo\Desktop\Universita\Terzo Anno\LSO\The_Body_and_Society_IT.md"
    
    print("Estrazione del libro dal file di log della conversazione...")
    raw_text = extract_book_from_transcript(transcript_path)
    if not raw_text:
        print("Errore: Testo del libro non trovato nel transcript!")
        exit(1)
        
    pages = parse_pages(raw_text)
    print(f"Trovate {len(pages)} pagine testuali. Inizio traduzione...")
    
    translate_pages(pages, output_md_path)
    print(f"\nTraduzione completata! File salvato in: {output_md_path}")
