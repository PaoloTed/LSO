import os
from reportlab.lib.pagesizes import A4
from reportlab.lib import colors
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.platypus import (
    SimpleDocTemplate, Paragraph, Spacer, Table, TableStyle, KeepTogether, HRFlowable
)
from reportlab.pdfgen import canvas

class NumberedCanvas(canvas.Canvas):
    def __init__(self, *args, **kwargs):
        super(NumberedCanvas, self).__init__(*args, **kwargs)
        self._saved_page_states = []

    def showPage(self):
        self._saved_page_states.append(dict(self.__dict__))
        self._startPage()

    def save(self):
        num_pages = len(self._saved_page_states)
        for state in self._saved_page_states:
            self.__dict__.update(state)
            self.draw_page_decorations(num_pages)
            super(NumberedCanvas, self).showPage()
        super(NumberedCanvas, self).save()

    def draw_page_decorations(self, page_count):
        self.saveState()
        self.setFont("Helvetica", 8)
        self.setFillColor(colors.HexColor("#718096"))
        
        # Header (pages after first)
        if self._pageNumber > 1:
            self.drawString(54, A4[1] - 36, "Laboratorio di Sistemi Operativi — Traccia A: Labirinto con oggetti e uscita")
            self.setStrokeColor(colors.HexColor("#E2E8F0"))
            self.setLineWidth(0.5)
            self.line(54, A4[1] - 42, A4[0] - 54, A4[1] - 42)
        
        # Footer
        footer_text = f"Pagina {self._pageNumber} di {page_count}"
        self.drawRightString(A4[0] - 54, 34, footer_text)
        self.drawString(54, 34, "Corso di Laurea in Informatica — A.A. 2025/2026")
        self.setStrokeColor(colors.HexColor("#E2E8F0"))
        self.setLineWidth(0.5)
        self.line(54, 46, A4[0] - 54, 46)
        
        self.restoreState()

def build_pdf(txt_path, pdf_path):
    doc = SimpleDocTemplate(
        pdf_path,
        pagesize=A4,
        leftMargin=54,
        rightMargin=54,
        topMargin=50,
        bottomMargin=54
    )

    styles = getSampleStyleSheet()

    header_tag_style = ParagraphStyle(
        'HeaderTag',
        parent=styles['Normal'],
        fontName='Helvetica-Bold',
        fontSize=8.5,
        leading=11,
        textColor=colors.HexColor("#2B6CB0"),
        spaceAfter=4,
        textTransform='uppercase'
    )

    title_style = ParagraphStyle(
        'MainTitle',
        parent=styles['Title'],
        fontName='Helvetica-Bold',
        fontSize=18,
        leading=22,
        textColor=colors.HexColor("#1A365D"),
        alignment=0,
        spaceAfter=12
    )

    h1_style = ParagraphStyle(
        'Heading1Custom',
        parent=styles['Heading1'],
        fontName='Helvetica-Bold',
        fontSize=12,
        leading=16,
        textColor=colors.HexColor("#2B6CB0"),
        spaceBefore=12,
        spaceAfter=6,
        keepWithNext=True
    )

    body_style = ParagraphStyle(
        'BodyCustom',
        parent=styles['Normal'],
        fontName='Helvetica',
        fontSize=9.8,
        leading=14.5,
        textColor=colors.HexColor("#2D3748"),
        alignment=4, # Justified
        spaceAfter=8
    )

    bullet_style = ParagraphStyle(
        'BulletCustom',
        parent=body_style,
        leftIndent=18,
        firstLineIndent=-12,
        spaceAfter=4
    )

    num_list_style = ParagraphStyle(
        'NumListCustom',
        parent=body_style,
        leftIndent=22,
        firstLineIndent=-16,
        spaceAfter=7
    )

    story = []

    # Title Banner
    story.append(Paragraph("Laboratorio di Sistemi Operativi — Specifica del Progetto", header_tag_style))
    story.append(Paragraph("TRACCIA A – Labirinto con oggetti e uscita", title_style))
    story.append(HRFlowable(width="100%", thickness=1.5, color=colors.HexColor("#2B6CB0"), spaceAfter=14))

    # Descrizione Sintetica
    story.append(Paragraph("Descrizione Sintetica", h1_style))
    story.append(Paragraph(
        "Realizzare un sistema client–server che consenta a più utenti di partecipare, contemporaneamente, "
        "a un gioco di esplorazione di un labirinto. Ogni giocatore si muove in una mappa 2D generata randomicamente, "
        "raccoglie oggetti e cerca di raggiungere l'uscita. La mappa è nota solo al server; per ciascun giocatore il server "
        "tiene traccia delle caselle via via scoperte. Ad ogni mossa il giocatore riceve una vista locale, e periodicamente "
        "riceve una mappa globale \"mascherata\". Il progetto va sviluppato in C su piattaforma UNIX con comunicazione tramite socket TCP e corredato da documentazione.",
        body_style
    ))
    story.append(Spacer(1, 4))

    # Descrizione Dettagliata
    story.append(Paragraph("Descrizione Dettagliata", h1_style))
    story.append(Paragraph(
        "All'avvio il server genera un labirinto rappresentato come una matrice di celle contenenti: muri, celle libere, "
        "alcune uscite e un certo numero di oggetti raccoglibili. Ogni giocatore che si connette viene posizionato in una cella libera casuale. "
        "Il giocatore vede inizialmente solo la propria cella.",
        body_style
    ))
    story.append(Paragraph(
        "Il giocatore può muoversi in una delle quattro direzioni. Quando entra in una cella contenente un oggetto lo raccoglie; "
        "l'oggetto viene rimosso dal server. Ogni volta che il giocatore si muove, il server \"rivela\" una zona attorno a lui, "
        "che diventa permanentemente scoperta per quel giocatore.",
        body_style
    ))
    story.append(Paragraph(
        "La partita termina allo scadere di un timeout globale oppure quando tutti i giocatori sono usciti. Il vincitore è determinato come segue:",
        body_style
    ))

    # Victory conditions
    story.append(Paragraph("• <b>se esce un solo giocatore</b> &rarr; vince lui;", bullet_style))
    story.append(Paragraph("• <b>se escono più giocatori prima del timeout</b> &rarr; vince chi ha raccolto più oggetti;", bullet_style))
    story.append(Paragraph("• <b>se nessuno esce</b> &rarr; vince chi ha raccolto più oggetti.", bullet_style))
    story.append(Spacer(1, 4))

    story.append(Paragraph(
        "Per accedere al gioco ogni utente dovrà prima registrarsi al sito indicando password e nickname. "
        "Non c'è un limite a priori al numero di utenti che si possono collegare con il server.",
        body_style
    ))
    story.append(Paragraph(
        "Il server mantiene l'intera mappa del gioco e, per ogni giocatore, l'insieme delle celle da lui scoperte. "
        "Quando riceve un comando (ad esempio un movimento), il server aggiorna la posizione del giocatore, rivela nuove celle e restituisce al client una mappa locale: una finestra (es. 4x4) centrata sul giocatore contenente solo le celle da lui scoperte.",
        body_style
    ))
    story.append(Paragraph(
        "Indipendentemente dalle azioni dei client, il server invia periodicamente (ogni T secondi) a ciascun giocatore una mappa globale mascherata (l'intera matrice di gioco), in cui le celle da lui non ancora viste sono indicate con un simbolo speciale. Il server deve gestire simultaneamente: nuove connessioni, comandi dei client già connessi, l'invio periodico della mappa globale, senza bloccare i client. Il server sarà di tipo concorrente, ovvero in grado di servire più client simultaneamente. Durante il suo regolare funzionamento, il server effettuerà il logging delle attività principali in un file apposito. Ad esempio, memorizzando la data e l'ora di connessione dei client e il loro nome simbolico (se disponibile, altrimenti l'indirizzo IP) e la data e l'ora delle conquiste.",
        body_style
    ))
    story.append(Paragraph(
        "Il client consentirà all'utente di collegarsi ad un server di comunicazione, indicando tramite riga di comando il nome o l'indirizzo IP di tale server e la porta da utilizzare. Una volta collegato ad un server l'utente potrà accedere al servizio come utente con un nickname. Il client permetterà all'utente di: spostarsi sulla mappa, disconnettersi, vedere la lista degli utenti collegati, visualizzare la mappa locale e quella globale.",
        body_style
    ))
    story.append(Spacer(1, 4))

    # Regole generali Box
    rg_elements = [
        Paragraph("Regole generali", h1_style),
        Paragraph(
            "Il server ed il client vanno realizzati in linguaggio C su piattaforma UNIX/Linux. Le comunicazioni tra client e server si svolgono tramite socket TCP.",
            body_style
        ),
        Paragraph(
            "Oltre alle system call UNIX, i programmi possono utilizzare solo la libreria standard del C. È sconsigliato l'uso di primitive non coperte dal corso al posto di quelle studiate. Il server non deve inviare alcun output su standard output, non deve ricevere nessun input da standard input e può inviare output su standard error solo in caso di terminazione.",
            body_style
        )
    ]
    rg_table = Table([[rg_elements]], colWidths=[doc.width])
    rg_table.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,-1), colors.HexColor("#F7FAFC")),
        ('BOX', (0,0), (-1,-1), 1, colors.HexColor("#E2E8F0")),
        ('LINELEFT', (0,0), (-1,-1), 3.5, colors.HexColor("#2B6CB0")),
        ('TOPPADDING', (0,0), (-1,-1), 8),
        ('BOTTOMPADDING', (0,0), (-1,-1), 8),
        ('LEFTPADDING', (0,0), (-1,-1), 12),
        ('RIGHTPADDING', (0,0), (-1,-1), 12),
    ]))
    story.append(rg_table)
    story.append(Spacer(1, 8))

    # Relazione
    rel_elements = [
        Paragraph("Relazione", h1_style),
        Paragraph("Il progetto va accompagnato da una relazione che contenga almeno le seguenti sezioni:", body_style),
        Paragraph("<b>1.</b> Una guida d'uso per il server e per il client, che illustri le modalità di compilazione e d'uso dei due programmi.", num_list_style),
        Paragraph("<b>2.</b> Una sezione che illustri il protocollo al livello di applicazione utilizzato nelle comunicazioni tra client e server (non il protocollo TCP/IP!).", num_list_style),
        Paragraph("<b>3.</b> Una sezione che descriva i dettagli implementativi giudicati più interessanti, eventualmente corredati dai corrispondenti frammenti di codice.", num_list_style),
        Paragraph("<b>4.</b> La relazione dovrebbe constare di circa 10 pagine. Indicare sulla copertina della relazione i componenti del gruppo.", num_list_style)
    ]
    story.append(KeepTogether(rel_elements))
    story.append(Spacer(1, 8))

    # Consegna del progetto Box
    cons_elements = [
        Paragraph("Consegna del progetto", ParagraphStyle(
            'HeadingConsegna', parent=h1_style, textColor=colors.HexColor("#285E61"), spaceBefore=2
        )),
        Paragraph(
            "Entro la data prescelta per lo scritto finale (con eccezione per il primo appello) devono essere consegnati al docente il progetto e la relazione. Il progetto e la relazione saranno inviati al docente, via chat di teams, in un archivio compresso in formato zip o rar. Durante l'esame orale, il client ed il server verranno testati, eseguendoli su due o più macchine diverse.",
            body_style
        )
    ]
    cons_table = Table([[cons_elements]], colWidths=[doc.width])
    cons_table.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,-1), colors.HexColor("#E6FFFA")),
        ('BOX', (0,0), (-1,-1), 1, colors.HexColor("#B2F5EA")),
        ('LINELEFT', (0,0), (-1,-1), 3.5, colors.HexColor("#319795")),
        ('TOPPADDING', (0,0), (-1,-1), 8),
        ('BOTTOMPADDING', (0,0), (-1,-1), 8),
        ('LEFTPADDING', (0,0), (-1,-1), 12),
        ('RIGHTPADDING', (0,0), (-1,-1), 12),
    ]))
    story.append(KeepTogether([cons_table]))

    doc.build(story, canvasmaker=NumberedCanvas)
    print(f"PDF generated successfully at {pdf_path}")

if __name__ == '__main__':
    base_dir = r"c:\Users\Paolo\Desktop\Universita\TerzoAnno\LSO\Progetto"
    txt = os.path.join(base_dir, "TRACCIAPROGETTO.txt")
    pdf = os.path.join(base_dir, "TRACCIAPROGETTO.pdf")
    build_pdf(txt, pdf)
