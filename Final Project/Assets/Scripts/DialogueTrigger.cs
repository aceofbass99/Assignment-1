using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DialogueTrigger : MonoBehaviour
{
    public Dialogue dialogue;
    private bool started = false;
    public void TriggerDialogue()
    {
        if(!started){
            FindObjectOfType<DialogueManager>().StartDialogue(dialogue);
            started = true;
        }else{
            FindObjectOfType<DialogueManager>().DisplayNextSentence();
        }
    }
}
