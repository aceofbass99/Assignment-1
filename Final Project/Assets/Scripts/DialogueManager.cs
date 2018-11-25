using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class DialogueManager : MonoBehaviour {
    public Text nameText;
    public Text dialogueText;
    private Queue<string> sentences;
    public Animator animator;
    public Text buttonText;
    // Use this for initialization
    public GameObject player;
	void Start () {
        animator.SetBool("IsOpen", true);
        sentences = new Queue<string>();
	}
    public void StartDialogue(Dialogue dialogue){

        buttonText.text = "Continue";
        nameText.text = dialogue.name;
        sentences.Clear();
        foreach(string sentence in dialogue.sentences){
            sentences.Enqueue(sentence);
        }
        DisplayNextSentence();

    }
    public void DisplayNextSentence(){

        if(sentences.Count == 0){
            EndDialogue();
            return;
        }
        string sentence = sentences.Dequeue();

        StopAllCoroutines();
        StartCoroutine(TypeSentence(sentence));
        if(sentences.Count == 0){
            buttonText.text = "Let's start";
        }
    }
    IEnumerator TypeSentence(string sentence){
        dialogueText.text = "";
        foreach(char letter in sentence.ToCharArray()){
            dialogueText.text += letter;
            yield return null;
        }
    }
    public void EndDialogue(){
        animator.SetBool("IsOpen", false);
        FindObjectOfType<PlayerControls>().startControl();
        FindObjectOfType<timer>().startCounting();
    }
}
